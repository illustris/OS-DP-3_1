all:
	g++ -c simulator.cpp
	g++ -c scheduler.cpp
	g++ scheduler.o simulator.o -o simulator
sched:
	g++ -c simulator.cpp
	g++ -c scheduler-$(s).cpp
	g++ scheduler-$(s).o simulator.o -o simulator
clean:
	rm -f simulator
	rm -f simulator.o
	rm -f scheduler*.o
run:
	g++ -c simulator.cpp
	g++ -c scheduler-$(s).cpp
	g++ scheduler-$(s).o simulator.o -o simulator
	./simulator tasklist
