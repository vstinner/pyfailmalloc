import failmalloc
import os
import unittest

class TestFailmalloc(unittest.TestCase):
    def tearDown(self):
        failmalloc.disable()

    def alloc_objects(self, count):
        return [object() for index in range(count)]

    def test_enable(self):
        count = 1000
        failmalloc.enable(count)
        self.assertRaises(MemoryError, self.alloc_objects, count)

    def test_disable(self):
        count = 1000
        failmalloc.enable(count)
        failmalloc.disable()
        # no MemoryError
        self.alloc_objects(count)

    def test_version(self):
        from importlib.machinery import SourceFileLoader

        filename = os.path.join(os.path.dirname(__file__), 'setup.py')
        loader = SourceFileLoader('setup', filename)
        setup_py = loader.load_module()
        self.assertEqual(failmalloc.__version__, setup_py.VERSION)


if __name__ == "__main__":
    unittest.main()

