#!/usr/bin/env python
# coding:utf-8

import re


def split_vertex(vertex_list):
    vertices = []
    vertex_pattern = re.compile(r'\(\-?\d+,\-?\d+\)')
    str_vertices = vertex_pattern.findall(vertex_list)
    for i in str_vertices:
        pattern = r'\((\-?\d+),(\-?\d+)\)'
        matchVer = re.match(pattern, i)
        if matchVer:
            vx = matchVer.group(1)
            vy = matchVer.group(2)
            v = Vertex(vx, vy)
            vertices.append(v)
    return vertices


class Runningerror(RuntimeError):  # Custom exception
    def __init__(self, arg):
        self.args = arg


class Vertex:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.num = -1  # The initial number is -1
        self.isIntersection = False  # The initial value is not the intersection point

    def sort_id(self, num):
        self.num = num

    def mark_Intersection(self):
        self.isIntersection = True

    def output_Vertex(self):
        print("  "+str(self.num) + ':  ({0:.2f}'.format(float(self.x)) + ',' + '{0:.2f})'.format(float(self.y)))


class Segment:
    def __init__(self, vs, ve):
        self.vs = vs  # Starting vertex
        self.ve = ve  # Ending vertex

    def seg_func(self):  # Get the parameters for calculating the intersection
        a = self.vs.y - self.ve.y
        b = self.ve.x - self.vs.x
        c = self.vs.x * self.ve.y - self.ve.x * self.vs.y
        seg_para = []
        seg_para.append(a)
        seg_para.append(b)
        seg_para.append(c)
        return seg_para


class Street:
    def __init__(self, name, vertices):
        self.name = name
        self.vertices = []
        if len(vertices) >= 2:  # At lease 2 vertices can generate a street.
            self.vertices = vertices

    def init_segment(self):  # Generate line segments every 2 vertices from the vertices list
        line_segment = []
        for i in range(len(self.vertices) - 1):
            ss = Vertex(float(self.vertices[i].x), float(self.vertices[i].y))
            se = Vertex(float(self.vertices[i + 1].x), float(self.vertices[i + 1].y))
            line_seg = Segment(ss, se)
            line_segment.append(line_seg)
        return line_segment


class Graph:
    def __init__(self):
        self.streets = {}
        self.vertices = []  # vertices: (a)intersection;
        #          (b)end-point of a line segment of a street that intersects with another street.
        self.edges = []  # edges if: (a) at least one of them is an intersection;
        #          (b) both lie on the same street;
        #          (c) one is reachable from the other without traversing another vertex.

    def add_street(self, cmd):
        pattern = r'a\s*\"([A-Za-z\s]*[A-Za-z])\"\s(((\(\-?\d+\,\-?\d+\))\s?)+)$'  # Alphabetical and space characters only(White-space considered)
        matchCmd = re.match(pattern, cmd, re.I)
        if matchCmd:
            str_name = matchCmd.group(1).lower() #str_name——Capitalization Insensitive
            vertex_list = matchCmd.group(2)
            if self.streets.has_key(str_name):
                print("Error: street currently exists.")
            else:
                str_vertices = split_vertex(vertex_list)
                self.streets[str_name] = Street(str_name, str_vertices)
        else:
            print("Error: Incorrect input format")

    def change_street(self, cmd):
        pattern = r'c\s*\"([A-Za-z\s]*[A-Za-z])\"\s(((\(\-?\d+\,\-?\d+\))\s?)+)$'  # Alphabetical and space characters only(White-space considered)
        matchCmd = re.match(pattern, cmd, re.I)
        if matchCmd:
            str_name = matchCmd.group(1).lower()
            if self.streets.has_key(str_name):  # Check if the street exists
                vertex_list = matchCmd.group(2)
                str_vertices = split_vertex(vertex_list)
                self.streets[str_name] = Street(str_name, str_vertices)
            else:
                print("Error: 'c' or 'r' specified for a street that does not exist.")
        else:
            print("Error: Incorrect input format")

    def remove_street(self, cmd):
        pattern = r'r\s*\"([A-Za-z\s]*[A-Za-z])\"$'  # Alphabetical and space characters only(White-space considered)
        matchCmd = re.match(pattern, cmd, re.I)
        if matchCmd:
            str_name = matchCmd.group(1).lower()
            if self.streets.has_key(str_name):  # Check if the street exists
                del self.streets[str_name]
            else:
                print("Error: 'c' or 'r' specified for a street that does not exist.")
        else:
            print("Error: Incorrect input format")

    def generate_graph(self):
        seg_comb = []  # All segment-intersection combos as [starting vertex,intersection, ending vertex] in, with repeating elements and needs update
        verticess = []  # All relevant points in with repeating elements and needs update
        str_names = self.streets.keys()
        for i in range(len(self.streets) - 1):
            for j in range(i + 1, len(self.streets)):
                i_name = str_names[i]
                i_street = Street(self.streets[i_name].name, self.streets[i_name].vertices)
                i_seg = i_street.init_segment()
                j_name = str_names[j]
                j_street = Street(self.streets[j_name].name, self.streets[j_name].vertices)
                j_seg = j_street.init_segment()
                # Each segment of any two streets matches each other for an intersection
                # using parameters calculating from the four coordinates of the two segments
                for si in range(len(i_seg)):
                    s1 = i_seg[si]
                    for sj in range(len(j_seg)):
                        s2 = j_seg[sj]
                        sp1 = s1.seg_func()
                        sp2 = s2.seg_func()
                        d = float(sp1[1] * sp2[0] - sp1[0] * sp2[1])
                        if d != 0:
                            x = float(float(sp2[1] * sp1[2] - sp1[1] * sp2[2]) / d)
                            y = float(float(sp1[0] * sp2[2] - sp2[0] * sp1[2]) / d)
                            if (min(s1.vs.x, s1.ve.x) <= x <= max(s1.vs.x, s1.ve.x)) & (
                                    min(s2.vs.x, s2.ve.x) <= x <= max(s2.vs.x, s2.ve.x)) & (
                                    min(s1.vs.y, s1.ve.y) <= y <= max(s1.vs.y, s1.ve.y)) & (
                                    min(s2.vs.y, s2.ve.y) <= y <= max(s2.vs.y, s2.ve.y)):
                                v_intersc = Vertex('%.2f' % x, '%.2f' % y)  # intersection
                                seg_comb.append([i_seg[si].vs, v_intersc, i_seg[si].ve])
                                seg_comb.append([j_seg[sj].vs, v_intersc, j_seg[sj].ve])
                        else:  # d==0, which means s1 and s2 are parallel
                            if (sp1[1]==0) & (sp2[1]==0) & (s1.vs.x==s2.vs.x): #s1 and s2 are on the same line, especially k=0 which means parallel to the y-axis
                                if (s2.vs.y <= s1.vs.y) & (s1.vs.y <= s1.ve.y) & (s1.ve.y <= s2.ve.y):
                                    seg_comb.append([s2.vs, s1.vs, s1.ve])
                                    seg_comb.append([s1.vs, s1.ve, s2.ve])
                                elif (s1.vs.y <= s2.vs.y) & (s2.vs.y <= s1.ve.y) & (s1.ve.y <= s2.ve.y):
                                    seg_comb.append([s1.vs, s2.vs, s1.ve])
                                    seg_comb.append([s2.vs, s1.ve, s2.ve])
                                elif (s1.vs.y <= s2.vs.y) & (s2.vs.y <= s2.ve.y) & (s2.ve.y <= s1.ve.y):
                                    seg_comb.append([s1.vs, s2.vs, s2.ve])
                                    seg_comb.append([s2.vs, s2.ve, s1.ve])
                                elif (s2.vs.y <= s1.vs.y) & (s1.vs.y <= s2.ve.y) & (s2.ve.y <= s1.ve.y):
                                    seg_comb.append([s2.vs, s1.vs, s2.ve])
                                    seg_comb.append([s1.vs, s2.ve, s1.ve])
                            if (sp1[1]!=0) & (sp2[1]!=0):
                                if (s1.vs.y + s1.vs.x * sp1[0] / sp1[1]) == (s2.vs.y + s2.vs.x * sp2[0] / sp2[1]):  # s1 and s2 are on the same line, and k!=0 which means unparallel to the y-axis
                                    if (s2.vs.x <= s1.vs.x) & (s1.vs.x <= s1.ve.x) & (s1.ve.x <= s2.ve.x):
                                        seg_comb.append([s2.vs, s1.vs, s1.ve])
                                        seg_comb.append([s1.vs, s1.ve, s2.ve])
                                    elif (s1.vs.x <= s2.vs.x) & (s2.vs.x <= s1.ve.x) & (s1.ve.x <= s2.ve.x):
                                        seg_comb.append([s1.vs, s2.vs, s1.ve])
                                        seg_comb.append([s2.vs, s1.ve, s2.ve])
                                    elif (s1.vs.x <= s2.vs.x) & (s2.vs.x <= s2.ve.x) & (s2.ve.x <= s1.ve.x):
                                        seg_comb.append([s1.vs, s2.vs, s2.ve])
                                        seg_comb.append([s2.vs, s2.ve, s1.ve])
                                    elif (s2.vs.x <= s1.vs.x) & (s1.vs.x <= s2.ve.x) & (s2.ve.x <= s1.ve.x):
                                        seg_comb.append([s2.vs, s1.vs, s2.ve])
                                        seg_comb.append([s1.vs, s2.ve, s1.ve])

        seg_comb_update = []  # All segment-intersection combos as [starting vertex,intersection, ending vertex] in, no repeating elements
        for sgc in seg_comb:
            seg_comb_update.append(sgc)
        for sc in range(len(seg_comb) - 1):
            for scc in range(sc + 1, len(seg_comb)):
                sgc1 = seg_comb[sc]
                sgc2 = seg_comb[scc]
                if (sgc1[0].x == sgc2[0].x) & (sgc1[0].y == sgc2[0].y) & (sgc1[1].x == sgc2[1].x) & (
                        sgc1[1].y == sgc2[1].y) & (sgc1[2].x == sgc2[2].x) & (sgc1[2].y == sgc2[2].y):
                    seg_comb_update.remove([sgc1[0], sgc1[1], sgc1[2]])

        # Generate V set of the graph
        for sc in range(len(seg_comb_update)):
            scb = seg_comb_update[sc]
            for c in range(len(scb)):
                verticess.append(scb[c])
        vtcs = []
        for i in verticess:
            if not i in vtcs:
                vtcs.append(i)
        del self.vertices[:]
        del self.edges[:]
        for vts in verticess:
            if vts not in self.vertices:
                self.vertices.append(vts)
        for vt in range(len(verticess)):
            item = verticess[vt]
            tx = float(item.x)
            ty = float(item.y)
            for vtt in range(vt + 1, len(verticess)):
                rx = float(verticess[vtt].x)
                ry = float(verticess[vtt].y)
                if (tx == rx) & (ty == ry):
                    for vtitem in self.vertices:
                        if (float(vtitem.x) == tx) & (float(vtitem.y) == ty):
                            self.vertices.remove(vtitem)
                    self.vertices.append(Vertex(tx, ty))
        for vts in self.vertices:
            for i in range(len(self.vertices)):
                self.vertices[i].num = i + 1

        # Generate E sets of the graph
        for sc in range(len(seg_comb_update)):
            scb = seg_comb_update[sc]
            for c in range(len(scb)):
                for vts in self.vertices:
                    if (float(scb[c].x) == vts.x) & (float(scb[c].y) == vts.y):
                        scb[c].num = vts.num
                        seg_comb_update[sc] = scb
        temp_edges=[]  # list of edges, with repeating elements and needs update to self.edges
        del_list = []  # Separating segments through two intersections to add the shortest edge
        scomb_box=[]   # As [starting vertex,intersection1,intersection2, ···, ending vertex]
        for i in range(len(seg_comb_update)):
            scb = seg_comb_update[i]
            sc_box=[]
            for j in range(i + 1, len(seg_comb_update)):
                sccb = seg_comb_update[j]
                if (scb[0].num == sccb[0].num) & (scb[2].num == sccb[2].num) & (scb[0].num != -1) & (scb[2].num != -1):
                    del_list.append(scb)
                    del_list.append(sccb)
                    if scb[0] not in sc_box:
                        sc_box.append(scb[0])
                    if scb[2] not in sc_box:
                        sc_box.append(scb[2])
                    if scb[1] not in sc_box:
                        sc_box.append(scb[1])
                    sc_box.append(sccb[1])
                continue
            if sc_box:
                scomb_box.append(sc_box)
        for ics in scomb_box:
            for icss in scomb_box:
                if (ics[0].num==icss[0].num) & (ics[len(ics)-1].num==icss[len(icss)-1].num) & (len(ics)>len(icss)):
                    scomb_box.remove(icss)
        for ics in scomb_box:
            n = len(ics)
            if (float(ics[0].x)==float(ics[n-1].x)):
                for i in range(1, n):
                    for j in range(i, 0, -1):
                        if float(ics[j].y) < float(ics[j - 1].y):
                            ics[j], ics[j - 1] = ics[j - 1], ics[j]
                        else:
                            break
            else:
                for i in range(1, n):
                    for j in range(i, 0, -1):
                        if float(ics[j].x) < float(ics[j - 1].x):
                            ics[j], ics[j - 1] = ics[j - 1], ics[j]
                        else:
                            break
        for ics in scomb_box:
            for i in range(len(ics)-1):
                temp_edges.append('<%d,%d>' % (ics[i].num, ics[i+1].num))
        for i in del_list:
            for j in seg_comb_update:
                if i == j:
                    seg_comb_update.remove(j)
        for i in range(len(seg_comb_update)):
            scb = seg_comb_update[i]
            temp_edges.append('<%d,%d>' % (scb[0].num, scb[1].num))
            temp_edges.append('<%d,%d>' % (scb[1].num, scb[2].num))
        for ed in temp_edges:
            if ed not in self.edges:
                self.edges.append(ed)

        # output graph
        print("V = {")
        for vts in self.vertices:
            vts.output_Vertex()
        print("}")
        print("E = {")
        comma = 0
        for e in self.edges:
            comma += 1
            if comma < len(self.edges):
                print("  "+str(e) + ",")
            else:  # last one don't print comma
                print("  "+str(e))
        print("}")


def main():
    g = Graph()
    while True:
        try:
            cmd = raw_input()
            if cmd[0] == "a":
                g.add_street(cmd)
            elif cmd[0] == "c":
                g.change_street(cmd)
            elif cmd[0] == "r":
                g.remove_street(cmd)
            elif cmd[0] == "g":
                g.generate_graph()
            else:
                raise Runningerror("Error: Incorrect input format")
        except Runningerror as e:
            print ("Error: Incorrect input format")
            break


if __name__ == '__main__':
    main()
