커널드라이브를 사용하지 않고 유저모드 상에서 프로세스에 변조 및 공격에 대해 방어하도록 구현한 프로젝트

구현기능

1. Anti Dll injection module
2. Anti Debugger
3. Section 영역 PE body부분에 대해 무결성 검사
4. 메모리 암호화 memory Encrpytion
5. Thread Manager -> thread 생성을 관리하는 하나의 thread를 생성하여 모든 쓰레드 생성 추적 -> anti dll에 대해 방지법으로 구현

Code Description
https://www.notion.so/Anti-Cheat-Module-8221f67b1e484b52841260090928e15a

사용 언어 모델
C++ 17 
