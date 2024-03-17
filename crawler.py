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
    for i in result:
        if i[0].isdigit():
            return

    lock.acquire()
    with open("train.txt", "a", encoding="utf-8") as f:
        if result != []:
            if head != "":
                # cont += head.upper() + str(no) + "\n"
                f.write(head.upper() + str(no) + "\n")
            else:
                # cont += "N" + str(no) + "\n"
                f.write("N" + str(no) + "\n")
            for i in result:
                # cont += i[0] + "\t" + i[1] + "\t" + i[2] + "\n"
                f.write(i[0] + "\t" + i[1] + "\t" + i[2] + "\n")
            # cont += "\n"
            f.write("\n")
    pbars[id].update(1)
    lock.release()


with open("train.txt", "w") as f:
    f.write("")
for i, head in enumerate(["g", "d", "c", "z", "t", "k", "s", "y", ""]):
    if head == "y":
        pbars.append(tqdm(total=23, desc=head.upper()))
    elif head == "":
        pbars.append(tqdm(total=9999, desc="N"))
    else:
        pbars.append(tqdm(total=9999, desc=head.upper()))
    for no in range(1, 10000) if head != "y" else range(752, 775):
        pool.submit(handler, head, no, i)
pool.shutdown()
# with open("train.txt", "a", encoding="utf-8") as f:
#     f.write(cont)
# cd "C:\Users\28956\Desktop\24-Spring\Discrete Mathematics(2)\12306" && conda activate dm
