# Dynminag-hashing
Implementation of Dynami hasing and performance comparison with chaning method




체이닝기법과 동적해싱에대한 성능비교를 봤을때 결과내용은 명확하다.
동적해싱은 체이닝기법보다 탐색하는데있어서 훨씬 빠른속도를 보장한다. 
하지만 구축하는데있어서 시간비용은 체이닝기법이 더 우세하다.
왜냐하면 동적해싱은 메모리카피를 요구하기때문이다. 메모리카피는 되게 비싼연산이고, 또한 메모리를 재대로 복사하려면 인스턴스 혹은 구조체에 대해서 깊은복사(deep copy)를 수반해야하기때문에 시간을 많이잡아먹는다.

ElapsedTime은 구축되는데 걸린시간임.즉, 자료를 특정모델에 맞게 삽입,삭제하는과정을 말함 (search time이 아님)
구축시간은 체이닝이 우세하지만 탐색시간은 동적해싱이 훨씬 빠름


동적해싱
<img width="550" alt="2018-11-24 3 16 02" src="https://user-images.githubusercontent.com/12508269/48956649-8711f000-ef97-11e8-9f9a-593e15da18c7.png">



체이닝
<img width="599" alt="2018-11-24 3 15 51" src="https://user-images.githubusercontent.com/12508269/48956648-85e0c300-ef97-11e8-8f03-69f677bc7d42.png">

