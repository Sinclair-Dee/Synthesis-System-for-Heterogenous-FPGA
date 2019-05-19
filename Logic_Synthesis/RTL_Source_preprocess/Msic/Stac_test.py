from lib import *

def test():
    s=Stack()
    print(s.isEmpty())
    s.push('a')
    s.push('b')
    print(s.size())
    print(s.pop())
    print(s.isEmpty())
    s.pop()
    print(s.isEmpty())
    

def main():
    test()

main()
