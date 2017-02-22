JC = javac
.SUFFIXES: .java .class
.java.class: 
	$(JC) $*.java

CLASSES = \
	Client.java \
	Server.java 

default: classes

classes: $(CLASSES:.java=.class)

clean: 
	$(RM) *.class
