f = open('test_map_mini.svg', 'r')
def parseMapObject(mapp):
    map_dict = dict()
    map_set = set()
    for data in mapp[1:]:
        #start_fig = data.find("<g")
        name = parseAttribute("id", data)
        if(name == ""): print("Error parse id"); return
        print(name)
        header_end = data.find(">")
        coords = parseObjectParts(data[header_end+1:])
        print(coords)
        map_dict[name] = coords
    return map_dict

def parseObjectParts(layout):
    parts = layout.split("<line")
    keys = ["x1", 'y1', 'x2', 'y2']
    parts_coords_list = []
    for data in parts[1:]:
        coords = []
        for key in keys:
            numb = parseAttribute(key, data)
            if(numb != ""):
                try:
                    numb = float(numb)
                    coords.append(numb)
                except:
                    print("Error parse coords " + numb)
            else:
                print("Error parse " + key)
        parts_coords_list.append(coords)
    return parts_coords_list
        
def parseAttribute(key, string, start=0):
    start_attr = string.find(key+"=\"", start)
    if(start_attr == -1):   
        return ""
    start_attr += len(key)+2
    fin_attr = string.find("\"", start_attr+1)
    if(fin_attr == -1):
        return ""
    return string[start_attr:fin_attr]
    
    
data = f.read()
f.close()
mapp = data.split("<g")
map_objects = parseMapObject(mapp)

out = open('test_map_decoded.txt', 'w')
for key in map_objects:
    out.write(key + " ")
    obj = map_objects[key]
    out.write(str(len(obj)) + "\n")
    for elem in obj:        
        out.write(" ".join(map(str, elem)) + "\n")
print("End")
out.close()

    

