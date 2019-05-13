#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define SLOT_SIZE 2

typedef int Data;

typedef struct _Node {
    Data data;
    struct _Node* prev;
    struct _Node* link;
}Node;

typedef struct _List {
    Node* head;
    Node* tail;
}List;

typedef struct _Bucket {
    List* list;
    int slotCnt;
}Bucket;

typedef struct _Directory {
    Bucket* bucketPointer;
    int doseHaveBucket;
    int index;
}Directory;

///// 鍮꾧탳瑜� �쐞�븳 泥댁씠�떇 援ъ“泥�
typedef struct _Table {
    List* list;
    int slotCnt;
}Table;
///


int depth = 1;  // 2쩔짬 징쨋�닞�녌올꺿늹횤�닞횑 짧�앪닞짖짬흹�닆횊쨉

// 5 == 10(1)
// 1 == 0(1)

int mypow(int num, int k) {
    int cnt = k, ret = 1;
    
    while (cnt--)
        ret *= num;
    
    return ret;
}

int extract(int* arr, int len) {
    // ex : arr = 1011
    // depth : 2
    // expected output : 3(11)
    int output = 0;
    
    for (int i = 1; i <= depth; i++) {
        if (arr[len - i])
            output += mypow(2, i - 1);
    }
    
    return output;
}


int* transformToBinary(int num, int* len) {
    int binary[20] = { 0, };
    int* temp;
    int d = depth - 1;
    
    int position = 0;
    while (1)
    {
        binary[position] = num % 2;    // 2�닊흸 �돟�꽓짜짤챈�쇑온� �늹횩 �돟�꽓�닆��씲￠녳닆쨋 ��횏첩�돖첩째 쩔�쇑올�
        num /= 2;             // 2�닊흸 �돟�꽓짜쨈 �닆횣쩔짧 쩔�쇑올�
        
        if (num == 0 || position == depth - 1) {    // �닆횣쩔횄 0쩔횄 쨉짬�닆횊 ����뷜닽��쩔짧 �돟째�돟첩
            if (d == 0 || position == depth - 1)
                break;
            else
                d--;
        }
        
        position++;    // 쩔�걚�닆짜쨘�� �닽횠�닞횎
    }

    position++;  // 廓짬징쨋�닊흸짜짭 +1쩔짧 짬첼챈詮� ��횏첩�돖쩔짬 length�닞째 쨉
    
    temp = (int*)malloc(sizeof(int)* (position));
    *len = position;
    
    // ��횏첩�돖쩔짬 첩��걔뷜�쒋닆쨋 첩�꽓쨘짱쩔�닆�닊흸 �닖��싢닊짭
    for (int i = position - 1, cnt = 0; i >= 0; i--) {
        temp[cnt++] = binary[i];
    }
    return temp;
}

int isBucketFull(Directory* dir, int idx) {
    if (dir[idx].bucketPointer->slotCnt == SLOT_SIZE)
        return TRUE;
    else
        return FALSE;
}

void createNode(Bucket* bp, int k) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = k;
    node->link = NULL;
    node->prev = NULL;
    
    if(bp->list->head == NULL) {
        bp->list->head = node;
        bp->list->tail = node;
    } else {
        bp->list->tail->link = node;
        node->prev = bp->list->tail;
        bp->list->tail = node;
    }
    
    bp->slotCnt++;
}

void createBucket(Directory* dir, int idx) {
    dir->bucketPointer = (Bucket*)malloc(sizeof(Bucket));
    dir->doseHaveBucket = TRUE;
    dir->bucketPointer->list = (List*)malloc(sizeof(List));
    dir->bucketPointer->list->head = NULL;
    dir->bucketPointer->list->tail = NULL;
    dir->bucketPointer->slotCnt = 0;
    dir->index = idx;
}

void linkNode(Bucket* bp, Node* node) {
    if(bp->list->head == NULL) {
        bp->list->head = node;
        bp->list->tail = node;
        node->prev = NULL;
    } else {
        bp->list->tail->link = node;
        node->prev = bp->list->tail;
        bp->list->tail = node;
    }
    bp->slotCnt++;
}

Data removeNode(Bucket* bp, int target) {
    int isFind = FALSE;
    Data ret;
    Node* head = bp->list->head;
    Node* tail = bp->list->tail;
    Node* traverse = head;
    
    while(traverse) {
        if(traverse->data == target) {
            isFind = TRUE;
            break;
        }
        traverse = traverse->link;
    }
    
    if(isFind == FALSE) return FALSE;
    
    if(traverse != head)
        traverse->prev->link = traverse->link;
    else
        
        bp->list->head = head->link;

    if(traverse != tail)
        traverse->link->prev = traverse->prev;
    else
        bp->list->tail = tail->prev;
        
    ret = traverse->data;
    bp->slotCnt--;
    free(traverse);
    
    return ret;
}

int findAnotherPlace(Directory dir[], int idx, int originIdx, int curKey) {
    int* binaryArr;
    int len = 0;
    int otherPlace;
    
    Node* traverse = dir[idx].bucketPointer->list->head;
    int classification[SLOT_SIZE+1]; // �옱遺꾨쪟�떆�궓 �씤�뜳�뒪�뱾�쓣 �떞怨좎엳�쓬 (湲곗〈�쓽踰꾩폆�뿉�엳�뒗 �뜲�씠�꽣媛��닔 + �깉濡쒖궫�엯�맂�뜲�씠�꽣1媛�)
    Node* data[SLOT_SIZE+1];  // �옱遺꾨쪟�맟�쓣�뻹 �씤�뜳�뒪�뿉 �빐�떦�븯�뒗 �뜲�씠�꽣 ����븳 �젙蹂대�쇰떞怨좎엳�쓬(�몮�떎 媛숈���닚�꽌���濡� 媛믪쓣 �떞湲곕븣臾몄뿉 �꽌濡쒕㏊�븨�씠�릺�뼱�엳�쓬)
    Node* head = traverse;
    Node* tail = dir[idx].bucketPointer->list->tail;
    int arrCnt = 0;
    int isFind = FALSE;
    
    
    createNode(dir[idx].bucketPointer, curKey); // ���梨꾧났媛꾩쓣 紐살갼�뒗�떎硫� �븿�닔留덉��留됱뿉�꽌 �궘�젣�떆耳쒖빞�븿.
    // 1. �삤踰꾪뵆濡쒕맂 踰꾩폆�쓽 紐⑤뱺 �뜲�씠�꽣��� �깉濡� �궫�엯�맂 �뜲�씠�꽣瑜� 紐⑤몢 �옱遺꾨쪟�떆�궓�떎.
    while(traverse) {
        binaryArr = transformToBinary(traverse->data, &len);
        /*
         ex :
         if(depth == 4) // 4 == 2^4
         7 = 0111
         5 = 0101
         11 = 1011
        */
        otherPlace = extract(binaryArr, len);
        data[arrCnt] = traverse;
        classification[arrCnt++] = otherPlace;
        
        traverse = traverse->link;
        len = 0;
    }
    
    // 2. 留뚯빟 1踰덉뿉�꽌 遺꾨쪟�맂 �뜲�씠�꽣以묒뿉�꽌 �쁽�옱 癒몃Ъ怨좎엳�뒗 �씤�뜳�뒪��� �옱遺꾨쪟�맂 �씤�뜳�뒪媛� �떎瑜대떎硫� �뵒�젆�넗由ъ궗�씠利덈�� �뒛由ъ���븡怨� �꽔�쓣怨듦컙�쓣 李얠븯�떎�뒗寃껋씠�떎.
    // 3. 2踰덉뿉�꽌 ���梨꾧났媛꾩쓣 李얠븯�떎硫� 遺꾨쪟寃곌낵瑜� �넗���濡� �뜲�씠�꽣瑜� �떎�떆 �꽔怨� 鍮쇰뒗 �옉�뾽�쓣 �븳�떎. �떒 �뜲�씠�꽣瑜� �꽔�쓣�븣, 洹� �씤�뜳�뒪媛� �옄�떊留뚯쓽 踰꾩폆�쓣 �냼�쑀�븯怨좎엳吏� �븡�떎硫� �깉濡� 留뚮뱾�뼱以��떎.
    for(int j=0; j<arrCnt; j++) {
        if(classification[j] != idx) {
            head = dir[idx].bucketPointer->list->head;
            tail = dir[idx].bucketPointer->list->tail;
            
            if(data[j] == head)
                dir[idx].bucketPointer->list->head = dir[idx].bucketPointer->list->head->link;
            else
                data[j]->prev->link = data[j]->link;
            
            if(data[j] != tail)
                data[j]->link->prev = data[j]->prev;
            else
                tail = tail->prev;
                
            data[j]->link = data[j]->prev = NULL;
            
            binaryArr = transformToBinary(data[j]->data, &len);
            otherPlace = extract(binaryArr, len);
    
            if(dir[otherPlace].doseHaveBucket == FALSE) {
                createBucket( &(dir[otherPlace]), otherPlace );
            }
            
            linkNode( dir[otherPlace].bucketPointer, data[j] );
            
            isFind = TRUE;
            dir[idx].bucketPointer->slotCnt--;\
        }
    }
    
    if(isFind == FALSE) {
        removeNode(dir[idx].bucketPointer, curKey);
    }
    
    return isFind;
}


void insert(Directory** dir, int k) {  // k : 쩔��섃닊짭�닞�꽓
    int len = 0;
    int* binaryArr = transformToBinary(k, &len);
    int idx = extract(binaryArr, len);
    int newPos = mypow(2, depth);
    int isFind;
    int prevIdx;
    
    if (isBucketFull(*dir, idx)) {
        // �뵒�젆�넗由� �궗�씠利덈�� �뒛由ъ���븡怨� �떎瑜멸납�뿉 梨꾩슱�닔�엳�떎硫� 洹멸납�뿉 梨꾩슦怨� TRUE瑜� 由ы꽩�븯怨� 洹몄쇅�뿉 FALSE 由ы꽩
        isFind = findAnotherPlace(*dir, (*dir)[idx].index , idx, k);  // 2踰덉�고뙆�씪誘명꽣�뒗 �쁽�옱�뵒�젆�꽣由ш�� 媛�由ы궎�뒗 踰꾩폆�쓽 �씤�뜳�뒪瑜쇰쭚�븿
        if(isFind) {
            return;
        }
        
        // �뵒�젆�꽣由� �궗�씠利덈�� �뒛�젮�빞�븳�떎硫� �뒛由ш퀬 �뜲�씠�꽣�궫�엯
        // depth瑜� �뒛由ш퀬 �깉濡쒖슫 媛믪뿉 ����빐�꽌 鍮꾪듃�닔瑜�1媛쒕뒛�젮�꽌 �떎�떆寃��궗�빐�꽌 �뱾�뼱媛덇났媛꾩갼�쓬.
        
        depth++;
//      *dir = (Directory*) realloc(*dir, sizeof(Directory)  * mypow(2, depth)  );
        Directory* newDir = (Directory*) malloc ( sizeof(Directory)  * mypow(2, depth)  );
        
        for(int i=0; i<mypow(2,depth-1); i++) {
            newDir[i].doseHaveBucket = (*dir)[i].doseHaveBucket;
            newDir[i].index = (*dir)[i].index;
            newDir[i].bucketPointer = (Bucket*)malloc(sizeof(Bucket));
            newDir[i].bucketPointer->list = (List*)malloc(sizeof(List));
            memcpy(newDir[i].bucketPointer, (*dir)[i].bucketPointer, sizeof(Bucket));
            memcpy(newDir[i].bucketPointer->list, (*dir)[i].bucketPointer->list, sizeof(List));
        }
        free(*dir);
        *dir = newDir;
        len = 0;
        binaryArr = transformToBinary(k, &len);
        idx = extract(binaryArr, len);
        
    
        for (int i = newPos; i < mypow(2, depth); i++) {
            (*dir)[i].bucketPointer = (*dir)[i - newPos].bucketPointer;
            (*dir)[i].doseHaveBucket = FALSE;
            (*dir)[i].index = i - newPos;
        }
        
        depth--;
        len = 0;
        binaryArr = transformToBinary(k, &len);
        prevIdx = extract(binaryArr, len);
        depth++;
        
        // �뱾�뼱媛덇났媛꾩씠 �삤踰꾪뵆濡쒓���릺�뒗吏� �븳踰덈뜑 泥댄겕�빐以섏빞�븿
        if(idx < mypow(2,depth-1)) {
            if (isBucketFull(*dir, idx)) {
                // �뿬湲곗꽑 �뵲濡� 遺꾧린臾몄씠 �븘�슂�뾾�쓬. �솢�깘�븯硫� �쐞�뿉�꽌 �씠誘� �뵒�젆�넗由ы솗�옣�쓣 吏꾪뻾�뻽湲곕븣臾몄뿉 ���梨꾧났媛꾩쓣 臾�.議�.嫄� 李얠븘�꽌 �뱾�뼱媛�.
                isFind = findAnotherPlace(*dir, (*dir)[idx].index , idx, k);
            }
        } else if(isBucketFull(*dir, prevIdx)) { // depth瑜� �뒛由ш린�쟾�뿉 �씤�뜳�뒪�븞�뿉 以묐났�맂�뜲�씠�꽣媛� �엳�쑝硫� 洹� 以묐났�맂�뜲�씠�꽣�룄 �떎媛숈씠�씠�룞�떆耳쒖쨲.
            isFind = findAnotherPlace(*dir, (*dir)[idx].index , idx, k);
        } else {
            createBucket( &((*dir)[idx]) , idx);
            createNode((*dir)[idx].bucketPointer, k);
        }
    }
    else {
        createNode((*dir)[idx].bucketPointer, k);
    }
}


void init(Directory** dir) {
    *dir = (Directory*)malloc(sizeof(Directory) * mypow(2,depth));
    
    for(int i=0; i< mypow(2, depth); i++)
        createBucket( &((*dir)[i]) , i);
}

void dump(Directory dir[]) {
    int size = mypow(2,depth);
    Node* traverse;
    
    printf("Directory size : %d\nSlot size : %d\n", size, SLOT_SIZE);
    
    for(int i=0; i<size; i++) {
        traverse = dir[i].bucketPointer->list->head;
        
        printf("[%d] : ", i);
        while(traverse) {
            printf("%d ,", traverse->data);
            traverse = traverse->link;
        }
        printf("\n");
    }
}

int search(Directory dir[], Data target, int flag) {
    int len = 0;
    int* binaryArr = transformToBinary(target, &len);
    int comp = 1;
    int idx = extract(binaryArr, len);
    Node* traverse = dir[idx].bucketPointer->list->head;
    
    while(traverse) {
        if(traverse->data == target) {
            if(flag)
                printf("comparision num of search \"%d\" : %d\n", target, comp);
            return comp;
        }
        traverse = traverse->link;
        comp++;
    }
    printf("Dose not exist\n");
    return 0;
}

void dump_omission_ver(Directory dir[]) {
    int size = mypow(2,depth);
    Node* traverse;
    
    printf("size : %d\n", size);
    
    for(int i=0; i<size; i++) {
        traverse = dir[i].bucketPointer->list->head;
        
        printf("[%d] : ", i);
        if(dir[i].doseHaveBucket) {
            while(traverse) {
                printf("%d ,", traverse->data);
                traverse = traverse->link;
            }
        }
        printf("\n");
    }
}

///////////////////////////////////
/////// 泥댁씠�떇 愿��젴 �븿�닔�뱾 /////////////
int TABLE_MAX;

void tableInit(Table** tb) {
    *tb = (Table*)malloc(sizeof(Table) * TABLE_MAX);
    
    for(int i=0; i<TABLE_MAX; i++) {
        (*tb)[i].slotCnt = 0;
        (*tb)[i].list = (List*)malloc(sizeof(List));
        (*tb)[i].list->head = NULL;
        (*tb)[i].list->tail = NULL;
    }
}

Data hash(Data k) {
    return k % TABLE_MAX;
}

void createTableNode(Table* tb, int k) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = k;
    node->link = NULL;
    node->prev = NULL;
    
    if(tb->list->head == NULL) {
        tb->list->head = node;
        tb->list->tail = node;
    } else {
        tb->list->tail->link = node;
        node->prev = tb->list->tail;
        tb->list->tail = node;
    }
    tb->slotCnt++;
}

void tableInsert(Table* tb, Data k) {
    int key = hash(k);
    createTableNode(&(tb[key]), k);
}

void dumpTable(Table* tb) {
    Node* traverse;
    for(int i=0; i<TABLE_MAX; i++) {
        traverse = tb[i].list->head;
        
        printf("[%d] : ", i);
        while(traverse) {
            printf("%d ,", traverse->data);
            traverse = traverse->link;
        }
        printf("\n");
    }
}

int searchTable(Table* tb, Data target, int flag) {
    int comp = 1;
    int k = hash(target);
    Node* traverse = tb[k].list->head;
    
    while(traverse) {
        if(traverse->data == target) {
            if(flag)
                printf("comparision num of search \"%d\" : %d\n\n", target, comp);
            return comp;
        }
        traverse = traverse->link;
        comp++;
    }
    return 0;
}

///////////////////////////////////




int main()
{
    /*
    int s[] = { 1,0,1 };
    depth = 3;
    printf("\n\n%d\n", extract(s, 3));
    */
    int compNum = 0;
    float gap;
    time_t startTime = 0, endTime = 0;
    
    startTime = clock();
    Directory* dir = NULL;
    init(&dir);
    /*
    insert(&dir, 8);
    insert(&dir, 12);
    insert(&dir, 7);
    insert(&dir, 6);
    insert(&dir, 5);
    insert(&dir, 11);
    insert(&dir, 3);
    insert(&dir, 3);
    */
    
    /*
    insert(&dir, 8);
    insert(&dir, 12);
    insert(&dir, 7);
    insert(&dir, 5);
    insert(&dir, 6);
    insert(&dir, 3);
    insert(&dir, 11);
    */
    
    
    /// �룞�쟻�빐�떛 寃곌낵
    /*
    for(int i=0; i<9000; i++) {
        insert(&dir, i);
    }
    dump(dir);
//    dump_omission_ver(dir);
    search(dir, 650, 1);
    search(dir, 720, 1);
    endTime = clock();
    gap = (float)(endTime-startTime)/(CLOCKS_PER_SEC);
    printf("Elapsed Time : %f\n", gap);
    for(int i=0; i<9000; i++)
        compNum += search(dir, i, 0);
    printf("Average num of comparisions : %d\n", compNum/9000);
    printf("Loading density : %f\n",  9000.0 / (SLOT_SIZE * mypow(2,depth)) );
    */
    
    /// 泥댁씠�떇 寃곌낵
    
    
    Table* tb;
    TABLE_MAX = 30;
    tableInit(&tb);
    printf("Chaining Result(0~8999)\n\n");
    for(int i=0; i<TABLE_MAX*300; i++) {
        tableInsert(tb, i);
//        tableInsert(tb,rand()%9000);
    }
    dumpTable(tb);
    printf("\n");
    searchTable(tb, 650,1);
    searchTable(tb, 720,1);
    endTime = clock();
    gap = (float)(endTime-startTime)/(CLOCKS_PER_SEC);
    printf("Elapsed Time : %f\n", gap);
    for(int i=0; i<9000; i++)
        compNum += searchTable(tb, i, 0);
    printf("Average num of comparisions : %d\n", compNum/9000);
    
    int slots = 0;
    for(int i=0; i<TABLE_MAX; i++)
        slots += tb[i].slotCnt;
    printf("Loading density : %f\n",  9000.0 / (((float)slots / (float)TABLE_MAX) * (float)TABLE_MAX) );  // a = n/sb  n = �뜲�씠�꽣媛��닔, s = �뒳濡�媛��닔, b = 踰꾩폆媛��닔
    
    return 0;
}

