import sys;

voewls = {"A", "E", "I", "O", "U"}
def is_vowel(ch):
    return ch in voewls

def possible_placement(s, idx):
    if len(s) < 3:
        return True

    if idx > 0 and idx < len(s)-1:
        a = is_vowel(s[idx-1])
        b = is_vowel(s[idx])
        c = is_vowel(s[idx+1])
        if (s[idx-1] != "_" and s[idx] != "_" and s[idx+1] != "_") and a == b and b == c and c == a:
            return False
    if idx > 1:
        a = is_vowel(s[idx-2])
        b = is_vowel(s[idx-1])
        c = is_vowel(s[idx])
        if (s[idx-2] != "_" and s[idx-1] != "_" and s[idx] != "_") and  a == b and b == c and c == a:
            return False
    if idx < len(s) - 3:
        a = is_vowel(s[idx])
        b = is_vowel(s[idx+1])
        c = is_vowel(s[idx+2])
        if  (s[idx] != "_" and s[idx+1] != "_" and s[idx+2] != "_") and  a == b and b == c and c == a:
            return False
    return True

def solve(s, holes, hole_idx, containL):
    if ((not containL) and hole_idx >= len(holes)):
        return 0
    if (hole_idx >= len(holes)):
        return 1

    idx = holes[hole_idx]
    sum = 0

    s[idx] = "L"

    if (possible_placement(s, idx)):
        if not containL:
            sum += 1 * solve(s,  holes,  hole_idx+1, True)
            sum += 20 * solve(s,  holes,  hole_idx+1, containL)
        else:
            sum += 21 * solve(s,  holes,  hole_idx+1, containL)
        # print ("L", idx)
        # tmp = 0
        # cL = containL
        # if cL == False:
        #     tmp = 1
        #     cL = True

        # else:
        #     tmp = 21
        # print(cL)

        # print(sum)

    s[idx] = "A"
    if (possible_placement(s, idx)):
        sum += 5 *  solve(s, holes, hole_idx+1, containL)

    s[idx] = "_"
    return sum



for line in sys.stdin:
    containL = False
    holes = []
    idx = 0
    s = []
    for c in line[:-1]:
        s.append(c)
        if c == "L":
            containL = True
        elif c == "_":
          holes.append(idx)
        idx += 1
    print(solve(s, holes, 0, containL))
