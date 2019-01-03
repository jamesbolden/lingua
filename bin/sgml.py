from bs4 import BeautifulSoup

def parseSGML(file):
    fd = open(file, "r", encoding="utf-8")
    bsObj = BeautifulSoup(fd)
    return [item.get_text() for item in bsObj.findAll("body")]
