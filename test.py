import unittest
import os
import subprocess
import tempfile
from pathlib import Path
import oschmod

class TestDatabase(unittest.TestCase):

    def setUp(self):
        self.original_cwd = os.getcwd()
        self.dbexe = Path(self.original_cwd) / 'build' / 'Debug' / 'db.exe'
        self.tmpdir = tempfile.TemporaryDirectory()
        os.chdir(self.tmpdir.name)

    def tearDown(self):
        os.chdir(self.original_cwd)

    def test_cannot_locate_dbfile_returns_error(self):
        self.assertFalse(os.path.exists('./database.db'))
        self.assertTrue(os.path.exists(self.dbexe))
        result = subprocess.run([str(self.dbexe)], input='exit', text=True)
        self.assertEqual(result.returncode, 1)

    def test_cannot_open_dbfile_returns_error(self):
        self.assertFalse(os.path.exists('./database.db'))
        with open('./database.db', 'w') as file:
            file.write('key value')
        self.assertTrue(os.path.exists('./database.db'))
        oschmod.set_mode('./database.db', 0o000)
        self.assertTrue(os.path.exists(self.dbexe))
        result = subprocess.run([str(self.dbexe)], input='exit', text=True)
        self.assertEqual(result.returncode, 1)

    def test_database_loads_and_exits_successfully(self):
        self.assertFalse(os.path.exists('./database.db'))
        with open('./database.db', 'w') as file:
            file.write('key value\n')
        self.assertTrue(os.path.exists('./database.db'))
        self.assertTrue(os.path.exists(self.dbexe))
        result = subprocess.run([str(self.dbexe)], input='exit', text=True)
        self.assertEqual(result.returncode, 0)

    def test_database_with_duplicate_keys_fails_to_load(self):
        self.assertFalse(os.path.exists('./database.db'))
        with open('./database.db', 'w') as file:
            file.write('key value1\n')
            file.write('key value2\n')
        self.assertTrue(os.path.exists('./database.db'))
        self.assertTrue(os.path.exists(self.dbexe))
        result = subprocess.run([str(self.dbexe)], input='exit', text=True)
        self.assertEqual(result.returncode, 1)

    def test_create_runs_successfuly(self):
        pass

    def test_create_with_duplicate_key_fails(self):
        pass

    def test_create_with_empty_key_fails(self):
        pass

    def test_create_with_empty_value_fails(self):
        pass

    def test_read_existing_value_runs_successfuly(self):
        pass

    def test_read_nonexistant_value_runs_successfuly(self):
        pass

    def test_read_with_empty_key_fails(self):
        pass

    def test_update_existing_key_runs_successfuly(self):
        pass

    def test_update_nonexistant_key_runs_successfuly(self):
        pass

    def test_update_with_empty_key_fails(self):
        pass

    def test_update_with_empty_value_fails(self):
        pass

    def test_delete_existing_key_runs_successfuly(self):
        pass

    def test_delete_nonexistent_key_runs_successfuly(self):
        pass

    def test_delete_with_empty_key_fails(self):
        pass
