f = open('test_map_rect.svg', 'r')
def parseMapObject(mapp):
    map_dict = dict()
    map_set = set()
    for data in mapp[1:]:
        #start_fig = data.find("<g")
        name = parseAttribute("id", data)
        if(name == ""): print("Error parse id"); return
        header_end = data.find(">")
        if("Passage" in name):
            coords = parseObjectParts(data[header_end+1:])
        elif("Room" in name):
            coords = parseRoom(data[header_end+1:])
        else:
            print("Wrong object:" + name)
            return;
        print(name)
        print(coords)
        if(coords != []):
            map_dict[name] = coords
        else:
            return {}
    return map_dict
def parseRoom(layout):
    parts = layout.split("<")
    parts_coords_list = [0]*6
    rect_ind = 0;
    coords = []
    for data in parts[1:]:
        if data.find("rect") >= 0:
            keys = ["x", "y", "width", "height"]
            coords = parseParamSet(keys, data)
            if(coords == []):
                print("Error parse rect")
                return []
            coords[2] += coords[0]  #x2
            coords[3] += coords[1]  #y2
            parts_coords_list[:4] = coords
        elif data.find("circle") >= 0:
            keys = ["cx", "cy"]
            coords = parseParamSet(keys, data)
            if(coords == []):
                print("Error parse circle")
                return [] 
            parts_coords_list[4] = coords[0]
            parts_coords_list[5] = coords[1]
        else:
            pass
    return [parts_coords_list]
def parseParamSet(keys, src):
    coords = []
    for key in keys:
        numb = parseAttribute(key, src)
        if(numb != ""):
            try:
                numb = float(numb)
                coords.append(round(numb, 1))
            except:
                print("Error parse coords " + numb)
                return []
        else:
            print("Error parse " + key + "\n" + src)
            return []
    return coords
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
w = 1000
h = 1000
out.write(str(w)+" " + str(h) + "\n")
for key in map_objects:
    out.write(key + " ")
    obj = map_objects[key]
    out.write(str(len(obj)) + "\n")
    for elem in obj:        
        out.write(" ".join(map(str, elem)) + "\n")
print("End")
out.close()

    

