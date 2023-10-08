

#ifndef OCTETOS_NEURONAL_V0_HH
#define OCTETOS_NEURONAL_V0_HH

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

#define infimium 0.0001
#define CU_ASSERT_DECIMAL(V,R,I) CU_ASSERT(abs(V - R) < infimium)

int v0_init(void);
int v0_clean(void);

void v0_developing();

#endif
