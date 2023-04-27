runner: SubProcesses/P1_Sigma_sm_gg_ttxg/check_sa.cc SubProcesses/P1_Sigma_sm_gg_ttxg/PEP.hpp SubProcesses/P1_Sigma_sm_gg_ttxg/PEPPER.hpp
	cd SubProcesses/P1_Sigma_sm_gg_ttxg/ && make -j
	export MG5AMC_CARD_PATH=Cards/
	alias runner.exe=./SubProcesses/P1_Sigma_sm_gg_ttxg/runner.exe

clean:
	cd SubProcesses/P1_Sigma_sm_gg_ttxg/ && make clean
	unset MG5AMC_CARD_PATH
	unalias runner.exe

cleanall:
	cd SubProcesses/P1_Sigma_sm_gg_ttxg/ && make cleanall
	unset MG5AMC_CARD_PATH
	unalias runner.exe
