import subprocess, os
import random

same = True
test_str = ''
cnt = 0
tcnt = 0

start_v = -2
end_v = 2
p1 = start_v
p2 = start_v
p3 = start_v
p4 = start_v

p5 = start_v
p6 = start_v
p7 = start_v
p8 = start_v

test_batch = 100


while same:
    #  get_test1 = subprocess.Popen('./a.out', stdout=subprocess.PIPE)
    #  test_str = get_test1.stdout.read()
    #
    test_str = str(test_batch) + "\n"

    for _ in range(0,test_batch):
        tup = [random.randint(-10, 10) for _ in range(0,8)]
        test_str += "%s %s %s %s %s %s %s %s\n" % tuple(tup)

    read1, write1 = os.pipe()
    os.write(write1, str.encode(test_str))
    os.close(write1)

    proc1 = subprocess.Popen('./main.out', stdin=read1, stdout=subprocess.PIPE)
    proc1_str = proc1.stdout.read()
    os.close(read1)

    read2, write2 = os.pipe()
    os.write(write2, str.encode(test_str))
    os.close(write2)

    proc2 = subprocess.Popen('./working.out', stdin=read2, stdout=subprocess.PIPE)
    proc2_str = proc2.stdout.read()
    os.close(read2)

    cnt = cnt + 1
    if cnt == 1000:
        tcnt = tcnt + 1
        cnt = 0
        print(tcnt, "k correct")
        print(test_str)
    same = proc2_str == proc1_str

print("Fail: ")
t_lines = test_str.split("\n")[1:]
self_lines = proc1_str.decode("utf-8").split("\n")
kactl_lines = proc2_str.decode("utf-8").split("\n")

for (q, a1, a2) in zip(t_lines, self_lines, kactl_lines):
    if (a1 != a2):
        print("%s ===> %s ||| %s\n" % (q, a1, a2))

#end thats all
