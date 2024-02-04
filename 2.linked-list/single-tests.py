import unittest

from single import List, Node


class TestCases(unittest.TestCase):

    def setUp(self):
        my_list = List()
        my_list.append(1)
        my_list.append(2)
        my_list.append(3)
        my_list.append(4)
        my_list.append(5)
        self.my_list = my_list

    def test_append_and_traverse(self):
        my_list = self.my_list
        print(my_list.traverse())
        self.assertEqual("12345", my_list.traverse())

    def test_shift_append(self):
        my_list = self.my_list
        my_list.shift()
        my_list.shift()

        self.assertEqual("345", my_list.traverse())

    def test_insert(self):
        my_list = self.my_list
        my_list.insert(1, 10)

        self.assertEqual("1210345", my_list.traverse())

    def test_unshift(self):
        my_list = self.my_list
        my_list.unshift()
        my_list.unshift()

        self.assertEqual("345", my_list.traverse())

    def test_pop(self):
        my_list = self.my_list
        my_list.pop()
        my_list.pop()

        self.assertEqual("123", my_list.traverse())

    def test_remove(self):
        my_list = self.my_list
        my_list.remove(0)
        my_list.remove(1)

        self.assertEqual("245", my_list.traverse())

    def test_find(self):
        my_list = self.my_list

        self.assertEqual(1, my_list.find(2))

    def test_get(self):
        my_list = self.my_list

        self.assertEqual(5, my_list.get(4))

    def test_length(self):
        my_list = self.my_list

        self.assertEqual(5, my_list.length)

    def test_flush(self):
        my_list = self.my_list

        my_list.flush()
        self.assertEqual(None, my_list.head)

    def test_reverse(self):
        my_list = self.my_list
        my_list.reverse()

        self.assertEqual("54321", my_list.traverse())
