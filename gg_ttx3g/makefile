runner: SubProcesses/P1_Sigma_sm_gg_ttxggg/check_sa.cc SubProcesses/P1_Sigma_sm_gg_ttxggg/PEP.hpp SubProcesses/P1_Sigma_sm_gg_ttxggg/PEPPER.hpp
	cd SubProcesses/P1_Sigma_sm_gg_ttxggg/ && make -j
	export MG5AMC_CARD_PATH=Cards/
	alias runner.exe=./SubProcesses/P1_Sigma_sm_gg_ttxggg/runner.exe

clean:
	cd SubProcesses/P1_Sigma_sm_gg_ttxggg/ && make clean
	unset MG5AMC_CARD_PATH
	unalias runner.exe

cleanall:
	cd SubProcesses/P1_Sigma_sm_gg_ttxggg/ && make cleanall
	unset MG5AMC_CARD_PATH
	unalias runner.exe
