#!/usr/bin/env python
#coding:utf-8

class Vertex:
	def __init__(self,x,y):
		self.x=x
		self.y=y
	def sort_num(self,num):
		self.num=num
	def mark_Intersection(self):
		self.isIntersection=True
	def mark_Endpoint(self):
		self.isEndpoint=True


class Segment:
	def __init__(self,vs,ve):
		self.vs=vs
		self.ve=ve
	
	def get_allvertices(self):
		all_vertices=[]
		k=(self.vs.y-self.ve.y)/(self.vs.x-self.ve.x)
		b=self.vs.y-k*self.vs.x
		#print k,b
		for nx in range(min(self.vs.x,self.ve.x),max(self.vs.x,self.ve.x)+1):
			#print nx
			ny=k*nx+b
			#print ny
			all_vertices.append((nx,ny))
		#print all_vertices
		return all_vertices

class Street:
	def __init__(self,name,vertices):
		self.name=name
		self.vertices=[]
		if len(vertices) >= 2: #At lease 2 vertices can generate a street.
			self.vertices=vertices

	def init_segment(self,vertices):
		line_segment=[]
		#print len(vertices)
		for i in range(len(vertices)-1):
			line_seg=Segment(vertices[i],vertices[i+1])
			line_segment.append(line_seg)
			print ((line_segment[i].vs.x,line_segment[i].vs.y),(line_segment[i].ve.x,line_segment[i].ve.y))
		return line_segment


'''

v1=Vertex(1,1)	
v2=Vertex(3,5)
v3=Vertex(4,7)
seg=Segment(v1,v2)
seg.get_allvertices()
vertices=[v1,v2,v3]
strt=Street("weber",vertices)
#print strt.name
#for i in range(len(strt.vertices)):
	#print (strt.vertices[i].x,strt.vertices[i].y)
strt.init_segment(vertices)

#if __name__ == '__main__':
	#main()

'''


