#!/usr/bin/env python
#coding:utf-8
 

class Vertex:
	def __init__(self,num,x,y):
		self.num=num
		self.x=x
		self.y=y

	def display_v(self):
		print '%d: (%d,%d)' %(self.num,self.x,self.y)

class Segment:
	def __init__(self,vs,ve):
		self.vs=vs
		self.ve=ve

	def seg(self):
		self.vs=Vertex(1,1,1)
		self.ve=Vertex(2,3,5)
		nodes_list=[]
		k=(self.vs.y-self.ve.y)/(self.vs.x-self.ve.x)
		b=self.vs.y-k*self.vs.x
		print k,b
		for nx in range(min(self.vs.x,self.ve.x),max(self.vs.x,self.ve.x)+1):
			print nx
			ny=k*nx+b
			print ny
			nodes_list.append((nx,ny))
		print nodes_list


v1=Vertex(1,1,1)	
v2=Vertex(2,3,5)	
v1.display_v()
v2.display_v()
s=Segment(v1,v2)
s.seg()


class Street:
	def __init__(self,name,nodes):
		self.name=name
		self.nodes=[]
		self.vertices=
		self.edges = {} 

class Graph:
	def __init__(self):
		self.street_dic={}
		self.vertices

def main():
	G=graph()
	try:
		execCmd(raw_input())
	except Exception, e:
		raise
	else:
		pass
	finally:
		pass

	display_graph():
		print "g'\'V = {'\'}"


if __name__ == '__main__':
	main()


