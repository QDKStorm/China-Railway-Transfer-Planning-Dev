import requests
import re
from concurrent.futures import ThreadPoolExecutor
import threading
from tqdm import tqdm

pool = ThreadPoolExecutor(max_workers=20)
lock = threading.Lock()
pbars = []


def handler(head, no, id):
    res = requests.get(f"https://www.chalieche.com/{head}{no}.htm")
    website = res.text

    result = re.findall(
        r'htm" target="_blank">(.*?)</a>.*?<span>(.*?)</span>.*?<span>(.*?)</span>',
        website,
        re.S,
    )
    if result != []:
        del result[0]
        del result[0]
        result[0] = list(result[0])
        result[0][1] = result[0][2]
        result[0] = tuple(result[0])
        result[-1] = list(result[-1])
        result[-1][2] = result[-1][1]
        result[-1] = tuple(result[-1])
        for r in result:
            if r[1] == "----" or r[2] == "----":
                return
        if len(result) < 2:
            return
        lock.acquire()
        with open("train.txt", "a") as f:
            if head != "":
                f.write(head.upper() + str(no) + "\n")
            else:
                f.write("N" + str(no) + "\n")
            for i in result:
                f.write(i[0] + "\t" + i[1] + "\t" + i[2] + "\n")
            f.write("\n")
        lock.release()
    pbars[id].update(1)


with open("train.txt", "w") as f:
    f.write("")
for i, head in enumerate(["g", "d", "c", "z", "t", "k", "s", "y", ""]):
# for i, head in enumerate(["g"]):
    if head != "y":
        pbars.append(tqdm(total=9999, desc=head))
    else:
        pbars.append(tqdm(total=23, desc=head))
    for no in range(1, 10000) if head != "y" else range(752, 775):
    # for no in range(5006, 5007):
        pool.submit(handler, head, no, i)
pool.shutdown()
# with open("train.txt", "a", encoding="utf-8") as f:
#     f.write(cont)
# cd "C:\Users\28956\Desktop\24-Spring\Discrete Mathematics(2)\12306" && conda activate dm
