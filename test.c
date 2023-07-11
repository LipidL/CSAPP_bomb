#include<stdio.h>
int edi, esi, edx, eax, ecx;

void func4(edi, esi, edx)
{
    eax = edx;
    eax -= esi;
    ecx = eax;
    eax = eax >> 31;
    eax += ecx;
    eax /= 2;
    ecx = eax + esi;
    if(ecx > edi){
        edx = ecx - 1;
        func4(edi, esi, edx);
        eax *= 2;
        return;
    }
    eax = 0;
    if(ecx >= edi){
        return;
    }
    esi = ecx + 1;
    func4(edi, esi, edx);
    eax = 2* eax + 1;
    return;
}

int main()
{
    int i;
    for(i = 0;i <= 14; i++){
        edi = i;
        esi = 0;
        edx = 0xe;
        func4(edi, esi, edx);
        printf("when edi = %d, return will be %d\n",edi, eax);
    }
    return 0;
}