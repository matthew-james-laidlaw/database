import unittest
import os
import subprocess
import tempfile
from pathlib import Path
import oschmod

class TestDatabase(unittest.TestCase):

    def test_cannot_locate_dbfile_returns_error(self):
        original_cwd = os.getcwd()
        dbexe = Path(original_cwd) / 'build' / 'Debug' / 'db.exe'
        with tempfile.TemporaryDirectory() as tmpdir:
            try:
                os.chdir(tmpdir)
                self.assertFalse(os.path.exists('./database.db'))
                self.assertTrue(os.path.exists(dbexe))
                result = subprocess.run([str(dbexe)])
                self.assertEqual(result.returncode, 1)
            finally:
                os.chdir(original_cwd)

    def test_cannot_open_dbfile_returns_error(self):
        original_cwd = os.getcwd()
        dbexe = Path(original_cwd) / 'build' / 'Debug' / 'db.exe'
        with tempfile.TemporaryDirectory() as tmpdir:
            try:
                os.chdir(tmpdir)
                self.assertFalse(os.path.exists('./database.db'))

                with open('./database.db', 'w') as file:
                    file.write('key value')

                self.assertTrue(os.path.exists('./database.db'))
                oschmod.set_mode('./database.db', 0o000)

                self.assertTrue(os.path.exists(dbexe))
                result = subprocess.run([str(dbexe)])
                self.assertEqual(result.returncode, 1)
            finally:
                os.chdir(original_cwd)

    def test_database_loads_successfully(self):
        original_cwd = os.getcwd()
        dbexe = Path(original_cwd) / 'build' / 'Debug' / 'db.exe'
        with tempfile.TemporaryDirectory() as tmpdir:
            try:
                os.chdir(tmpdir)
                self.assertFalse(os.path.exists('./database.db'))

                with open('./database.db', 'w') as file:
                    file.write('key value\n')

                self.assertTrue(os.path.exists('./database.db'))
                self.assertTrue(os.path.exists(dbexe))
                result = subprocess.run([str(dbexe)])
                self.assertEqual(result.returncode, 0)
            finally:
                os.chdir(original_cwd)

    def test_database_with_duplicate_keys_fails_to_load(self):
        original_cwd = os.getcwd()
        dbexe = Path(original_cwd) / 'build' / 'Debug' / 'db.exe'
        with tempfile.TemporaryDirectory() as tmpdir:
            try:
                os.chdir(tmpdir)
                self.assertFalse(os.path.exists('./database.db'))

                with open('./database.db', 'w') as file:
                    file.write('key value1\n')
                    file.write('key value2\n')

                self.assertTrue(os.path.exists('./database.db'))
                self.assertTrue(os.path.exists(dbexe))
                result = subprocess.run([str(dbexe)])
                self.assertEqual(result.returncode, 1)
            finally:
                os.chdir(original_cwd)
