import re
sum = 0 

while True:
    try:
        a = input()
    except EOFError:
        break
    res = re.findall("-?\d+", a)
    for i in res:
        i = int(i)
        sum += i

print(sum)
