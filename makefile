
default: bin/suprem

run: bin/suprem data/suprem.uk
	./bin/suprem

test-exam1: bin/suprem data/suprem.uk
	valgrind ./bin/suprem < ./examples/exam1/boron.in

test-exam2: bin/suprem data/suprem.uk
	valgrind ./bin/suprem < ./examples/exam2/oed.in

test-exam3: bin/suprem data/suprem.uk
	valgrind ./bin/suprem < ./examples/exam3/oed.in

bin/suprem: src/suprem
	mkdir -p bin
	cp src/suprem bin/suprem

bin/keyread: src/keyread/keyread
	mkdir -p bin
	cp src/keyread/keyread bin/keyread

data/suprem.uk: bin/keyread
	./bin/keyread data/suprem.uk < data/suprem.key

src/suprem:
	$(MAKE) -C ./src suprem

src/keyread/keyread:
	$(MAKE) -C ./src/keyread keyread

clean:
	find -name '*.o' -exec 'rm' '-v' '{}' \;
	find -path './*/*' -a -executable -a -type f  -exec 'rm' '-v' '{}' \;
