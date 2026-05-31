"""
Unit tests for the C Gym Tracker program.

How to use:
1. Save the C program as gym_tracker.c in the same folder where you run this
   test file, or pass its location with:
      PowerShell: $env:GYM_SOURCE="C:\\path\\to\\gym_tracker.c"
      bash/zsh:   export GYM_SOURCE=/path/to/gym_tracker.c
2. Run:
      python -m unittest -v test_gym_tracker.py

These tests use only Python's standard library. They compile the C file with
gcc, run menu flows with simulated keyboard input, and compile a small C
harness to test helper functions directly.
"""

import os
import shutil
import subprocess
import tempfile
import textwrap
import unittest
from pathlib import Path


def find_source() -> Path:
    env_source = os.environ.get("GYM_SOURCE")
    candidates = []

    if env_source:
        candidates.append(Path(env_source))

    candidates.extend(
        [
            Path.cwd() / "gym_tracker.c",
            Path.cwd() / "main.c",
            Path.cwd() / "gym.c",
        ]
    )

    for candidate in candidates:
        if candidate.is_file():
            return candidate.resolve()

    c_files = sorted(Path.cwd().glob("*.c"))
    if len(c_files) == 1:
        return c_files[0].resolve()

    raise FileNotFoundError(
        "No C source file found. Save the program as gym_tracker.c, "
        "or set GYM_SOURCE to the full path of the .c file."
    )


class GymTrackerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.gcc = shutil.which("gcc")
        if not cls.gcc:
            raise unittest.SkipTest("gcc is required to compile the C program")
        cls.source_path = find_source()

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.build_dir = Path(self.tmp.name)
        self.source_copy = self.build_dir / "gym_tracker.c"
        self.source_copy.write_text(
            self.source_path.read_text(encoding="utf-8", errors="replace"),
            encoding="utf-8",
        )
        self.app_path = self.compile_app()

    def tearDown(self):
        self.tmp.cleanup()

    def csv_file(self) -> Path:
        return self.build_dir / "examples" / "archivogin.csv"

    def compile_app(self) -> Path:
        exe = self.build_dir / ("gym_tracker.exe" if os.name == "nt" else "gym_tracker")
        result = subprocess.run(
            [
                self.gcc,
                "-std=c11",
                "-Wall",
                "-Wextra",
                "-Wno-unused-result",
                "-o",
                str(exe),
                "gym_tracker.c",
            ],
            cwd=self.build_dir,
            text=True,
            capture_output=True,
        )
        self.assertEqual(result.returncode, 0, result.stderr)
        return exe

    def run_app(self, stdin_text: str, timeout: int = 5) -> subprocess.CompletedProcess:
        return subprocess.run(
            [str(self.app_path)],
            cwd=self.build_dir,
            input=stdin_text,
            text=True,
            capture_output=True,
            timeout=timeout,
        )

    def assert_app_ok(self, result: subprocess.CompletedProcess) -> str:
        output = result.stdout + result.stderr
        self.assertEqual(result.returncode, 0, output)
        return output

    def test_menu_option_11_exits(self):
        output = self.assert_app_ok(self.run_app("11\n"))
        self.assertIn("Saliendo", output)

    def test_menu_rejects_invalid_option(self):
        output = self.assert_app_ok(self.run_app("99\n"))
        self.assertIn("Opcion invalida", output)

    def test_volumen_success_shows_details_and_total(self):
        output = self.assert_app_ok(self.run_app("1\n2\n10\n50\n"))
        self.assertIn("Set: 2", output)
        self.assertIn("Reps: 10", output)
        self.assertIn("Peso: 50.0kg", output)
        self.assertIn("Volumen total de Ejercicio: 1000.0kg", output)

    def test_volumen_rejects_invalid_inputs(self):
        cases = [
            ("1\n0\n", "Error de input"),
            ("1\n2\n0\n", "Error de input"),
            ("1\n2\n10\n-5\n", "Peso no valido"),
        ]
        for stdin_text, expected in cases:
            with self.subTest(stdin_text=stdin_text):
                output = self.assert_app_ok(self.run_app(stdin_text))
                self.assertIn(expected, output)

    def test_mul_ejer_calculates_total_volume(self):
        output = self.assert_app_ok(self.run_app("2\n2\n3\n10\n40\n4\n8\n25\n"))
        self.assertIn("Total volume: 2000.0", output)

    def test_mul_ejer_rejects_invalid_count(self):
        output = self.assert_app_ok(self.run_app("2\n0\n"))
        self.assertIn("Cantidad invalida", output)

    def test_sentinel_ejer_stops_at_zero_and_prints_total(self):
        output = self.assert_app_ok(self.run_app("3\n1\n2\n10\n50\n0\n"))
        self.assertIn("Entrenamiento finalizado", output)
        self.assertIn("Total volume: 1000.0", output)

    def test_promedio_peso_sorts_and_averages(self):
        output = self.assert_app_ok(self.run_app("4\n3\n80\n70\n90\n"))
        self.assertIn("Con orden:", output)
        self.assertIn("#1 70.0", output)
        self.assertIn("#2 80.0", output)
        self.assertIn("#3 90.0", output)
        self.assertIn("Tu peso promedio es 80.0", output)

    def test_promedio_peso_rejects_invalid_count(self):
        output = self.assert_app_ok(self.run_app("4\n0\n"))
        self.assertIn("Cantidad invalida", output)

    def test_buscar_max_min_peso_reports_values_and_positions(self):
        output = self.assert_app_ok(self.run_app("5\n4\n75\n82.5\n70\n82\n"))
        self.assertIn("Tu peso maximo es 82.5 en la posicion #2", output)
        self.assertIn("Tu peso minimo es 70.0 en la posicion #3", output)

    def test_buscar_max_min_peso_rejects_invalid_count(self):
        output = self.assert_app_ok(self.run_app("5\n0\n"))
        self.assertIn("Cantidad invalida", output)

    def test_buscar_peso_finds_value_after_sort(self):
        output = self.assert_app_ok(self.run_app("6\n4\n90\n70\n80\n100\n80\n"))
        self.assertIn("Esta en la posicion #2", output)
        self.assertIn("70, 80, 90, 100", output)

    def test_buscar_peso_reports_missing_value(self):
        output = self.assert_app_ok(self.run_app("6\n3\n10\n20\n30\n25\n"))
        self.assertIn("Peso no encontrado", output)

    def test_buscar_peso_rejects_invalid_count(self):
        output = self.assert_app_ok(self.run_app("6\n0\n"))
        self.assertIn("Cantidad invalida", output)

    def test_nombre_ejer_prints_name(self):
        output = self.assert_app_ok(self.run_app("7\nSentadilla\n"))
        self.assertIn("Nombre de ejercicio: Sentadilla", output)

    def test_name_peso_lists_each_exercise_with_weight(self):
        output = self.assert_app_ok(
            self.run_app("8\n2\nBench\n3\n10\n60\nSquat\n4\n8\n100\n")
        )
        self.assertIn("Bench 60.0 Kg", output)
        self.assertIn("Squat 100.0 Kg", output)

    def test_name_peso_rejects_invalid_count(self):
        output = self.assert_app_ok(self.run_app("8\n0\n"))
        self.assertIn("Cantidad invalida", output)

    def test_archivo_ejer_creates_csv_and_appends_exercises(self):
        output = self.assert_app_ok(
            self.run_app("9\n2\nBench press\n3\n10\n60\nSquat\n4\n8\n100\n")
        )

        csv_file = self.csv_file()
        self.assertTrue(csv_file.exists())

        csv_text = csv_file.read_text(encoding="utf-8")
        self.assertTrue(csv_text.startswith("id,fecha,ejercicio,sets,reps,peso,nota"))
        self.assertIn(",Bench press,3,10,60.00,1800.00", csv_text)
        self.assertIn(",Squat,4,8,100.00,3200.00", csv_text)
        self.assertIn("TOTAL,,,,5000.00", csv_text)
        self.assertIn("Total volume: 5000.0", output)

    def test_archivo_ejer_rejects_invalid_count(self):
        csv_file = self.csv_file()
        csv_file.parent.mkdir(parents=True, exist_ok=True)

        csv_file.write_text(
            "id,fecha,ejercicio,sets,reps,peso,nota\n",
            encoding="utf-8",
        )

        output = self.assert_app_ok(self.run_app("9\n0\n"))
        self.assertIn("Cantidad invalida", output)

    def test_leer_archivo_prints_existing_csv(self):
        csv_file = self.csv_file()
        csv_file.parent.mkdir(parents=True, exist_ok=True)

        csv_file.write_text(
            "id,fecha,ejercicio,sets,reps,peso,nota\n"
            "1,30/05/2026,Bench,3,10,60.00,1800.00\n"
            "TOTAL,,,,1800.00\n",
            encoding="utf-8",
        )

        output = self.assert_app_ok(self.run_app("10\n"))
        self.assertIn("id\tfecha\tejercicio\tsets\treps\tpeso\tnota", output)
        self.assertIn("1\t30/05/2026\tBench\t3\t10\t60.00\t1800.00", output)
        self.assertIn("TOTAL", output)

    def test_helper_functions_with_c_harness(self):
        harness = self.build_dir / "unit_harness.c"
        harness.write_text(
            textwrap.dedent(
                r'''
                #include <stdio.h>
                #include <stdlib.h>

                #define main gym_tracker_original_main
                #include "gym_tracker.c"
                #undef main

                #define ASSERT_TRUE(expr) do { \
                    if (!(expr)) { \
                        fprintf(stderr, "Assertion failed: %s at line %d\n", #expr, __LINE__); \
                        return 1; \
                    } \
                } while (0)

                static int feq(float a, float b) {
                    float diff = a - b;
                    if (diff < 0) diff = -diff;
                    return diff < 0.001f;
                }

                int main(void) {
                    float swap_values[] = {1.0f, 2.0f, 3.0f};
                    swap(swap_values, 0, 2);
                    ASSERT_TRUE(feq(swap_values[0], 3.0f));
                    ASSERT_TRUE(feq(swap_values[2], 1.0f));

                    float sort_values[] = {3.5f, -1.0f, 2.2f, 2.2f, 0.0f};
                    quick_sort(sort_values, 0, 4);
                    ASSERT_TRUE(feq(sort_values[0], -1.0f));
                    ASSERT_TRUE(feq(sort_values[1], 0.0f));
                    ASSERT_TRUE(feq(sort_values[2], 2.2f));
                    ASSERT_TRUE(feq(sort_values[3], 2.2f));
                    ASSERT_TRUE(feq(sort_values[4], 3.5f));

                    int found = binary_search(sort_values, 2.2f, 0, 4);
                    ASSERT_TRUE(found >= 0);
                    ASSERT_TRUE(feq(sort_values[found], 2.2f));
                    ASSERT_TRUE(binary_search(sort_values, 99.0f, 0, 4) == -1);

                    FILE *ids = fopen("ids.csv", "w");
                    ASSERT_TRUE(ids != NULL);
                    fprintf(ids, "id,fecha,ejercicio,sets,reps,peso,nota\n");
                    fprintf(ids, "3,30/05/2026,Bench,3,10,60.00,1800.00\n");
                    fprintf(ids, "TOTAL,,,,1800.00\n");
                    fprintf(ids, "7,30/05/2026,Squat,4,8,100.00,3200.00\n");
                    fclose(ids);
                    ASSERT_TRUE(obtener_ultimo_id("ids.csv") == 7);
                    ASSERT_TRUE(obtener_ultimo_id("missing.csv") == 0);

                    FILE *input = fopen("volume_input.txt", "w");
                    ASSERT_TRUE(input != NULL);
                    fprintf(input, "2\n10\n50\n");
                    fclose(input);
                    ASSERT_TRUE(freopen("volume_input.txt", "r", stdin) != NULL);

                    float peso = 0.0f;
                    int sets = 0;
                    int reps = 0;
                    float total = volumen(0, &peso, &sets, &reps);
                    ASSERT_TRUE(feq(total, 1000.0f));
                    ASSERT_TRUE(feq(peso, 50.0f));
                    ASSERT_TRUE(sets == 2);
                    ASSERT_TRUE(reps == 10);

                    return 0;
                }
                '''
            ),
            encoding="utf-8",
        )

        exe = self.build_dir / ("unit_harness.exe" if os.name == "nt" else "unit_harness")
        compile_result = subprocess.run(
            [
                self.gcc,
                "-std=c11",
                "-Wall",
                "-Wextra",
                "-Wno-unused-result",
                "-o",
                str(exe),
                "unit_harness.c",
            ],
            cwd=self.build_dir,
            text=True,
            capture_output=True,
        )
        self.assertEqual(compile_result.returncode, 0, compile_result.stderr)

        run_result = subprocess.run(
            [str(exe)],
            cwd=self.build_dir,
            text=True,
            capture_output=True,
        )
        self.assertEqual(run_result.returncode, 0, run_result.stdout + run_result.stderr)


if __name__ == "__main__":
    unittest.main(verbosity=2)
    