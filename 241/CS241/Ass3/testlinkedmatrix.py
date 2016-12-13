from linkedmatrix import LinkedMatrix
from node import Node
import unittest

class TestLinkedMatrix(unittest.TestCase):
	def setUp(self):
		self.x = 5
		self.y = 5
		self.lm = LinkedMatrix(self.x, self.y, 7)

	def test_str(self):
		self.assertEquals(str(self.lm), "7 7 7 7 7 \n7 7 7 7 7 \n7 7 7 7 7 \n7 7 7 7 7 \n7 7 7 7 7 \n")

	def test_getitem(self):
		self.assertEquals(self.lm[(3,2)], 7)

	def test_setitem(self):
		self.lm[(3,2)] = 17
		self.assertEquals(self.lm[(3,2)], 17)

	def test_iter(self):
		self.lm[(3,2)] = 17
		tr = [7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 17, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7]
		tl = []
		for it in self.lm:
			tl.append(it)	
		self.assertEquals(tl, tr)

	def test_insertRowAfter(self):
		self.lm[(4,4)] = 23
		self.lm.insertRowAfter(2)
		self.assertEquals(self.lm[(4,5)], 23)

	def test_insertColumnAfter(self):
		self.lm[(4,4)] = 23
		self.lm.insertColumnAfter(3)
		self.assertEquals(self.lm[(5,4)], 23)

	def test_removeRowAfter(self):
		self.lm[(4,4)] = 23
		self.lm.removeRowAfter(2)
		self.assertEquals(self.lm[(4,3)], 23)
	
	def test_removeColumnAfter(self):
		self.lm[(4,4)] = 23
		self.lm.removeColumnAfter(1)
		self.assertEquals(self.lm[(3,4)], 23)

if __name__ == "__main__":
	unittest.main()
