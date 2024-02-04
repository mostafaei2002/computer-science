class Node:
    def __init__(self, val):
        self.val = val
        self.next = None


class List:
    def __init__(self):
        self.head = None

    # Insertion
    def shift(self):
        self.head = self.head.next
        return self

    def append(self, val):
        new_node = Node(val)
        if self.head == None:
            self.head = new_node
            return self

        current = self.head
        while current.next:
            current = current.next

        current.next = new_node

        return self

    def insert(self, index, val):
        counter = 0
        current = self.head
        new_node = Node(val)

        while True:
            if counter == index:
                new_node.next = current.next
                current.next = new_node

            counter += 1
            current = current.next

            if not (current and current.next):
                break

        return "out of index"

    # Deletion
    def unshift(self):
        self.head = self.head.next

    def pop(self):
        current = self.head
        while current and current.next and current.next.next:
            current = current.next

        current.next = None

    def remove(self, index):
        current = self.head
        counter = 0
        while True:
            if index == 0:
                self.head = current.next
                return

            if counter == index - 1:
                current.next = current.next.next
                return

            counter += 1
            current = current.next
            if not (current and current.next):
                break

    # Search
    def find(self, val):
        current = self.head
        counter = 0
        while current and current.next:
            if current.val == val:
                break
            counter += 1
            current = current.next

        return counter

    def get(self, index):
        counter = 0
        current = self.head

        while True:
            if counter == index:
                return current.val

            if not (current and current.next):
                break
            else:
                counter += 1
                current = current.next

    # Reverse
    def reverse(self):
        current = self.head
        tmp = None

        while True:
            next_node = current.next

            current.next = tmp
            tmp = current

            current = next_node

            if next_node == None:
                self.head = tmp
                break

    # Length
    @property
    def length(self):
        counter = 1
        current = self.head
        while current and current.next:
            counter += 1
            current = current.next
        return counter

    def traverse(self):
        current = self.head
        res = ""

        while current and current.next:
            res += str(current.val)
            current = current.next
        res += str(current.val)

        return res

    def flush(self):
        self.head = None
