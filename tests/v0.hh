

#ifndef OCTETOS_NEURONAL_V0_HH
#define OCTETOS_NEURONAL_V0_HH

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#define infimium 0.0001
#define CU_ASSERT_DECIMAL(V,R,I) CU_ASSERT(abs(V - R) < infimium)

int v1_init(void);
int v1_clean(void);

void v1_developing();
void v1_Gate_AND();

#endif
