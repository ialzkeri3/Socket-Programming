# Contents for Java Makefile
JFLAGS = -g
JC = javac
JV = java

recompile: clean all

all: server client


server: server.java
	$(JC) $(JFLAGS) $<

client: client.java 
	$(JC) $(JFLAGS) $<

run-server:
	java server 127.1 9012

run-client:
	java client 127.1 9012

clean:
	rm *.class