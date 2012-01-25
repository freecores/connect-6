GCC=g++

SCC=schroot -c centos5-i386 ~/bin/synphonycc_fpga
SCC_SCRIPTS=./scc_scripts
SYNTH_SRC=./synth_src





all: fpt_connect6 test_golden

fpt_connect6: ${SYNTH_SRC}/main.cpp util.o connect6.o connect6_synth.o state.o threats.o
	${GCC} -o connect6 ${SYNTH_SRC}/main.cpp util.o connect6.o connect6_synth.o state.o threats.o  -lpthread -lm -g -pg

connect6_synth.o: ${SYNTH_SRC}/connect6_synth.cpp ${SYNTH_SRC}/connect6_synth.h
	${GCC} -c -g -pg ${SYNTH_SRC}/connect6_synth.cpp
connect6.o: ${SYNTH_SRC}/connect6.cpp ${SYNTH_SRC}/connect6.h
	${GCC} -c -g -pg ${SYNTH_SRC}/connect6.cpp

util.o: ${SYNTH_SRC}/util.cpp ${SYNTH_SRC}/util.h
	${GCC} -c -g -pg ${SYNTH_SRC}/util.cpp
state.o:${SYNTH_SRC}/state.cpp ${SYNTH_SRC}/shared.h
	${GCC} -c -g -pg ${SYNTH_SRC}/state.cpp
	
threats.o:${SYNTH_SRC}/threats.cpp ${SYNTH_SRC}/shared.h
	${GCC} -c -g -pg ${SYNTH_SRC}/threats.cpp


test_golden:
	./connect6 -player L

clean:
	rm -f *.o connect6 gmon.out myboard.txt




#first the c/c++ source should be compiled
fpga: synth altera_synth test
	cp cp ./rtl_package/synth/altera_fpga/run/DE2.sof ../DEMO

synth: all imp_connect
imp_window:
	#sed -s 's/\\TCAB_NAME/threat_window/g' main.cpp.base >main.cpp
	cp ${SYNTH_SRC}/main.cpp.base ${SYNTH_SRC}/main.cpp
	${SCC} ${SCC_SCRIPTS}/run_imp_window.tcl
imp_line: imp_window
	#sed -s 's/\\TCAB_NAME/threat_line/g' main.cpp.base >main.cpp
	cp ${SYNTH_SRC}/main.cpp.base ${SYNTH_SRC}/main.cpp
	${SCC} ${SCC_SCRIPTS}/run_imp_line.tcl
imp_marks:
	#sed -s 's/\\TCAB_NAME/ai_mraks/g' main.cpp.base >main.cpp
	cp ${SYNTH_SRC}/main.cpp.base ${SYNTH_SRC}/main.cpp
	${SCC} ${SCC_SCRIPTS}/run_imp_marks.tcl
imp_threat: imp_line imp_marks
	#sed -s 's/\\TCAB_NAME/ai_threats/g' main.cpp.base >main.cpp
	cp ${SYNTH_SRC}/main.cpp.base ${SYNTH_SRC}/main.cpp
	${SCC} ${SCC_SCRIPTS}/run_imp_threat.tcl
imp_adjacent:
	cp ${SYNTH_SRC}/main.cpp.base ${SYNTH_SRC}/main.cpp
	#sed -s 's/\\TCAB_NAME/ai_adjacent/g' main.cpp.base >main.cpp
	${SCC} ${SCC_SCRIPTS}/run_imp_adjacent.tcl
imp_connect: imp_threat  imp_adjacent
	sed -s 's/\\TCAB_NAME/connect6ai_synth/g' ${SYNTH_SRC}/main.cpp.base >${SYNTH_SRC}/main.cpp
	${SCC} ${SCC_SCRIPTS}/run_imp_connect.tcl

altera_synth:
	cp -r ./imp_connect/rtl_package/ ./
	cp DE2/do_synth ./rtl_package/synth/altera_fpga/
	cp DE2/quartus.tcl ./rtl_package/synth/altera_fpga/
	cp DE2/constraints.sdc ./rtl_package/synth/altera_fpga/
	echo -e 'all:\n\t./do_synth' > ./rtl_package/synth/altera_fpga/makefile
	make -C ./rtl_package/synth/altera_fpga/

test: 
	../scripts/serial_port_setup.sh
	rm -f ./test.result
	for i in `seq 1 100` ; do  ../connect6 -port /dev/ttyS0 -player L >> test.result 2>&1; done
	#grep "FPGA has won" ./test.result | wc -l
	#grep "AI has won" ./test.result | wc -l
	#grep "TIE" ./test.result | wc -l
	python ../scripts/esult.py
pgm:
	cp ./rtl_package/synth/altera_fpga/run/DE2.sof ./
	quartus_pgm -c USB-Blaster -m jtag  -o "p;DE2.sof"


prof:
	valgrind --tool=callgrind  ./connect6 -player L
	kcachegrind
	
altera_clean:
	rm -rf ./rtl_package/
	
ultraclean: altera_clean
	rm -rf imp_line imp_marks imp_threat imp_choose imp_adjacent imp_connect imp_window Logs
