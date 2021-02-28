#!/usr/bin/env python
#coding:utf-8

import re
from Street import init_segment

def split_vertex(vertex_list):
	vertex_pattern=re.compile(r'\(\-?\d,\-?\d\)') 
	str_vertices=vertex_pattern.findall(vertex_list)
	return str_vertices

class Graph:
	def __init__(self):
		self.streets={} 
		self.vertices=[] #vertices: (a)intersection, 
		                 #          (b)end-point of a line segment of a street that intersects with another street.
		self.edges=[] #edges if: (a) at least one of them is an intersection, 
		              #          (b) both lie on the same street, and, 
		              #          (c) one is reachable from the other without traversing another vertex.

	def add_street(self,cmd):
		pattern=r'a\s\"([A-Za-z\s]*[A-Za-z])\"\s((\(\-?\d,\-?\d\)\s*)+)$'
		matchCmd=re.match(pattern,cmd,re.I)
		if matchCmd:
			str_name=matchCmd.group(1)
			vertex_list=matchCmd.group(2)
			str_vertices=split_vertex(vertex_list)
			self.streets[str_name]=str_vertices
			print(self.streets)
		else:
			print("Wrong input.")


	def change_street(self,cmd):
		pattern=r'c\s\"([A-Za-z\s]*[A-Za-z])\"\s((\(\-?\d,\-?\d\)\s*)+)$'
		matchCmd=re.match(pattern,cmd,re.I)
		if matchCmd:
			str_name=matchCmd.group(1)
			if self.streets.has_key(str_name): #检查self.streets里是否有str_name这个key(该条路是否存在)
				vertex_list=matchCmd.group(2)
				str_vertices=split_vertex(vertex_list)
				self.streets[str_name]=str_vertices
			else:
				print("Non-existent street.")
		else:
			print("Wrong input.")


	def remove_street(self,cmd): 
		pattern=r'r\s\"([A-Za-z\s]*[A-Za-z])\"$'
		matchCmd=re.match(pattern,cmd,re.I)
		if matchCmd:
			str_name=matchCmd.group(1)
			if self.streets.has_key(str_name): #检查self.streets里是否有str_name这个key(该条路是否存在)
				del self.streets[str_name]
			else:
				print("Non-existent street.")
		else:
			print("Wrong input.")

	#def generate_graph(self):
		#generate self.vertices
		#generate self.edges
	def find_intersection(self):
		strs_test={}
		strs_test['Weber Street']=['(2,-1)', '(2,2)', '(5,5)', '(5,6)', '(3,8)']
		strs_test['King Street S']=['(4,2)', '(4,8)']
		self.streets=strs_test
		str_names=self.streets.keys()
		print str_names[0]
		print self.streets[str_names[0]]
		for i in range(len(self.streets)-1):
			for j in range(i+1,len(self.streets)):
				i_vertex=Street(str_names[i],self.streets[str_names[i]])
				print i_vertex.name
				print i_vertex.vertices
				j_vertex=Street(str_names[j],self.streets[str_names[j]])
				i_seg=init_segment(i_vertex.vertices)
				j_seg=init_segment(j_vertex.vertices)
				for x in range(len(i_seg)):
					i_allvertices=get_allvertices(i_seg[x])
				for y in range(len(j_seg)):
					j_allvertices=get_allvertices(i_seg[y])
				print i_allvertices
				print j_allvertices
				for a in range(len(i_allvertices)):
					for b in range(len(j_allvertices)):
						if i_allvertices[a]==j_allvertices[b]:
							i_allvertices[a].isIntersection=True
							self.vertices.append(i_allvertices[a])


	"""def generate_graph(self,vertices,edges):
		print('V={')
        for v in self.vertices:
            print(v)
        print('}')
        print('E={')
        count = 0
        for e in self.edges:
            count += 1
            if count < len(self.edges): # no comma after the last edge
                print(str(e) + ',')
            else:  
                print(str(e))
        print('}')
        """


g=Graph()

'''
cmd=raw_input()
if cmd[0]=="a":
	g.add_street(cmd)
elif cmd[0]=="c":
	g.change_street(cmd)
elif cmd[0]=="r":
	g.remove_street(cmd)
elif cmd[0]=="g":
	g.generate_graph()
else:
	print('Error. Input one of a/c/r/g command.')


strs_test={}
strs_test['Weber Street']=['(2,-1)', '(2,2)', '(5,5)', '(5,6)', '(3,8)']
strs_test['King Street S']=['(4,2)', '(4,8)']
'''


g.find_intersection()