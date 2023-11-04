
#include <cstring>
#include <string>


#ifdef OCTETOS_NEURONAL_V1_TTD
	#include "v1.hh"
#endif


int main(int argc, char *argv[])
{
    std::string mode;
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "--mode") == 0)
        {
            mode = argv[++i];
        }
    }

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

#ifdef OCTETOS_NEURONAL_V1_TTD
	CU_pSuite pSuite_v1 = CU_add_suite("Testing for Neuronal v1", v1_init, v1_clean);
	if (NULL == pSuite_v1)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	/*if ((NULL == CU_add_test(pSuite_v1, "Developing..", v1_developing)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}*/

	if ((NULL == CU_add_test(pSuite_v1, "NN for AND gate..", v1_Gate_AND)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	/*if ((NULL == CU_add_test(pSuite_v1, "NN for OR gate..", v1_Gate_OR)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}*/

	/*if ((NULL == CU_add_test(pSuite_v1, "NN for OR/AND gate..", v1_Gate_ORAND)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}*/

#endif

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}

