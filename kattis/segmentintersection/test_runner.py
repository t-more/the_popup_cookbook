import subprocess, os

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

while same:
    #  get_test1 = subprocess.Popen('./a.out', stdout=subprocess.PIPE)
    #  test_str = get_test1.stdout.read()
    #
    test_str = "1 "+str(p1)+" "+str(p2)+" "+str(p3)+" "+str(p4)+" "+str(p5)+" "+str(p6)+" "+str(p7)+" "+str(p8)

    read, write = os.pipe()
    os.write(write, str.encode(test_str))
    os.close(write)

    proc1 = subprocess.Popen('./main.out', stdin=read, stdout=subprocess.PIPE)
    proc1_str = proc1.stdout.read()
    os.close(read)

    read, write = os.pipe()
    os.write(write, str.encode(test_str))
    os.close(write)

    proc2 = subprocess.Popen('./working.out', stdin=read, stdout=subprocess.PIPE)
    proc2_str = proc2.stdout.read()
    os.close(read)

    cnt = cnt + 1
    if cnt == 1000:
        tcnt = tcnt + 1
        cnt = 0
        print(tcnt, "k correct")
        print(test_str)
    same = proc2_str == proc1_str

    p1 += 1
    if p1 > end_v:
        p1 = start_v
        p2 += 1
    if p2 > end_v:
        p2 = start_v
        p3 += 1
    if p3 > end_v:
        p3 = start_v
        p4 += 1
    if p4 > end_v:
        p4 = start_v
        p5 += 1
    if p5 > end_v:
        p5 = start_v
        p6 += 1
    if p6 > end_v:
        p6 = start_v
        p7 += 1
    if p7 > end_v:
        p7 = start_v
        p8 += 1
    if p8 > end_v:
        print("all tests passed")
        break;

print("Fail: ")
print(test_str)
 
#end thats all
