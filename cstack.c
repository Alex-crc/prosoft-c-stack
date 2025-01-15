#include "cstack.h"
#include <stddef.h>

#define UNUSED(VAR) (void)(VAR)

// Includes
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// узел представляющий один элемент данных стека
typedef struct node
{
    struct node* prev; // указатель на предыдущий узел
    unsigned int size; // размер данных в байтах
    uint8_t* pData;    // укзаатель на данные
} node;

typedef struct
{   
    bool stackEn; // stack enable
    unsigned int numElem; // кол-во эл-ов в стеке
    node* nodeTop; // указатель на "верхний" узел стека
} stack;

stack* stackTable = NULL;		// Динамический массив из "заголовков" стеков
unsigned int SizeOfTable = 10;	// Размер динамического массива

hstack_t stack_new(void)
{
    // Первоначальная инициализация stackTable
    if( stackTable == NULL )
    {
        stackTable = (stack*)malloc( SizeOfTable * sizeof( stack ) );

        if( stackTable == NULL )
        {
            return -1;
        }
        else
        {
            for( unsigned int i = 0; i < SizeOfTable; ++i)
            {
                stackTable[i].stackEn = false;
                stackTable[i].numElem = 0;
                stackTable[i].nodeTop = NULL;
            }
        }
    }

    // Ищем невыделенный заголовок под стек
    for( unsigned int i = 0; i < SizeOfTable; ++i )
    {
        if( stackTable[i].stackEn == false ) 
        {
            stackTable[i].stackEn = true;
            return i; 
        }
    }

    // Если все заголовки под стек заняты, запрашиваем доп. память под заголовки 
    unsigned int SizeOfTable_old = SizeOfTable;
    SizeOfTable += 10;
    stack* stackTmp = (stack*)realloc( stackTable, SizeOfTable * sizeof( stack ) );

    if( stackTmp == NULL )
    {
        return -1;
    }
    else
    {
        stackTable = stackTmp;

        for( unsigned int i = SizeOfTable_old; i < SizeOfTable; ++i)
        {
            stackTable[i].stackEn = false;
            stackTable[i].numElem = 0;
            stackTable[i].nodeTop = NULL;
        }

        stackTable[SizeOfTable_old].stackEn = true;
        return SizeOfTable_old; 
    }

    //return -1;
}

void stack_free(const hstack_t hstack)
{
    if( stackTable == NULL ) { return; }
    if( hstack < 0 || hstack > (int)SizeOfTable ) { return; }
    if( stackTable[hstack].stackEn == false ) { return; }

    if( stackTable[hstack].numElem != 0 )
    {
        while( stackTable[hstack].numElem > 0 )
        {
            node* tmp = stackTable[hstack].nodeTop;
            stackTable[hstack].nodeTop = stackTable[hstack].nodeTop->prev;
            free( tmp->pData );
            free( tmp );

            stackTable[hstack].numElem--;
        }
    }

    stackTable[hstack].stackEn = false;

    //UNUSED(hstack);
}

int stack_valid_handler(const hstack_t hstack)
{
    if( stackTable == NULL ) { return 1; }
    if( hstack < 0 || hstack > (int)SizeOfTable ) { return 1; }
    if( stackTable[hstack].stackEn == false ) { return 1; }

    return 0;

    //UNUSED(hstack);
    //return 1;
}

unsigned int stack_size(const hstack_t hstack)
{
    if( stackTable == NULL ) { return 0; }
    if( hstack < 0 || hstack > (int)SizeOfTable ) { return 0; }
    if( stackTable[hstack].stackEn == false ) { return 0; }

    return stackTable[hstack].numElem;

    //UNUSED(hstack);
    //return 0;
}

void stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    if( stackTable == NULL ) { return; }
    if( hstack < 0 || hstack > (int)SizeOfTable ) { return; }
    if( stackTable[hstack].stackEn == false ) { return; }

    if( data_in == NULL || size == 0 ) { return; }

    node* n = (node*)malloc( sizeof( node ) );
    if( n == NULL ) { return; }

    uint8_t* data = (uint8_t*)malloc( size );
    if( data == NULL )
    {
        free( n );
        return;
    }

    memcpy( data, data_in, size );

    n->prev  = stackTable[hstack].nodeTop;
    n->size  = size;
    n->pData = data;

    stackTable[hstack].nodeTop = n;
    stackTable[hstack].numElem++;

    //UNUSED(hstack);
    //UNUSED(data_in);
    //UNUSED(size);
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if( stackTable == NULL ) { return 0; }
    if( hstack < 0 || hstack > (int)SizeOfTable ) { return 0; }
    if( stackTable[hstack].stackEn == false ) { return 0; }

    if( data_out == NULL || size == 0 ) { return 0; }
    
    if( stackTable[hstack].numElem == 0 ) { return 0; }
    if( stackTable[hstack].nodeTop->size > size ) { return 0; }
    
    memcpy( data_out, stackTable[hstack].nodeTop->pData, size );

    node* tmp = stackTable[hstack].nodeTop;
    stackTable[hstack].nodeTop = stackTable[hstack].nodeTop->prev;
    stackTable[hstack].numElem--;
    free( tmp->pData );
    free( tmp );

    return size;

    //UNUSED(hstack);
    //UNUSED(data_out);
    //UNUSED(size);
    //return 0;
}


