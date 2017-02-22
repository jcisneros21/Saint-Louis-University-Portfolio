JC = javac
.SUFFIXES: .java .class
.java.class: 
	$(JC) $*.java

CLASSES = \
	Client.java \
	Server.java \
	Phase.java

default: classes

classes: $(CLASSES:.java=.class)

clean: 
	$(RM) *.class
