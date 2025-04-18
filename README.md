# CalFix
## 1. 프로젝트 개요
CalFix는 일정을 효율적으로 관리할 수 있도록 돕는 일정 관리 프로그램입니다.</br>
기본적인 일정 등록, 수정, 삭제, 검색 기능을 제공하며, 반복 일정 설정 기능 및 캘린더 위젯을 통해 일정을 간편하게 관리할 수 있도록 설계되었습니다.

## 2. 프로젝트 설계
### UML
![qt_scheduler_UML](https://github.com/user-attachments/assets/e3ed5c16-c697-412e-b992-36ce8d72dba3)
### ERD
![image](https://github.com/user-attachments/assets/3c6c1440-cda8-44d3-a522-a453fe9d4879)
## 2. 설치 방법
### 프로그램 다운로드
1. 이 저장소의 상단에서 Code 버튼을 클릭하고, Download ZIP을 선택합니다.</br>
또는 Releases 탭에서 최신 버전을 다운로드할 수 있습니다.
2. 압축을 해제한 후, scheduler.exe 를 더블클릭하여 실행합니다.
### 소스코드로부터 직접 빌드하기 (Qt 설치 필요)
1. Git을 사용해 프로젝트를 클론합니다:</br>
```bash
git clone https://github.com/kky107102/qt_scheduler.git
cd qt_scheduler
```
2. Qt Creator를 실행한 뒤, `CMakeLists.txt` 파일을 엽니다.
3. Qt Creator에서 키트를 선택하고, 실행 ▶️ 버튼을 클릭하여 프로그램을 빌드 및 실행합니다.

## 3. 사용법
### 캘린더
![image](https://github.com/user-attachments/assets/9ad7a46b-f25d-42ec-b257-2d7f59120187)
### 일정 검색
![image](https://github.com/user-attachments/assets/cc668b97-ba5a-4076-bae1-f666308100f7)
### 일별 일정 조회
![image](https://github.com/user-attachments/assets/4caa6fe5-3fed-4ba1-88d9-1bc6dead3a95)
### 일정 추가
![image](https://github.com/user-attachments/assets/d98249d8-f46f-4b35-a947-7f555ed52ee2)
### 일정 수정, 조회
![image](https://github.com/user-attachments/assets/2f705fe8-d695-4b9f-b3ce-b72ef7054125)

## 4. 기능
- 📆 월별 일정 조회
달력을 통해 한 달간의 모든 일정을 한눈에 확인할 수 있습니다. 일정이 있는 날에는 색칠하여 표시합니다.
- 📃 일별 일정 조회
선택한 날짜의 일정을 리스트 형태로 조회할 수 있습니다. 보기 버튼을 클릭하여 일정의 상세 내용을 확인할 수 있습니다.
- 📅 일정 추가
사용자가 원하는 날짜와 시간에 새로운 일정을 등록할 수 있습니다.
- 🔁 반복 일정 등록
매주 또는 매월 반복되는 일정을 등록할 수 있습니다.
- 📝 일정 수정
이미 등록된 일정의 제목, 시간, 장소, 메모 등을 수정할 수 있습니다.
- ❌ 일정 삭제
완료하였거나 불필요한 일정을 삭제할 수 있습니다.
- 🔍 일정 검색 (이름 기반)
일정 제목을 입력하여 등록된 일정을 리스트 형태로 조회할 수 있습니다. 항목을 클릭하여 일정의 상세 내용을 확인할 수 있습니다.
- 📍 메모 및 장소 추가 기능
일정마다 장소 정보와 상세 메모를 기록할 수 있어, 일정에 대한 부가정보를 관리할 수 있습니다.

## 5. 라이선스
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

### 서드-파티 라이브러리
This application uses the **Qt framework**, which is licensed under the **GNU Lesser General Public License (LGPL) v3**.

- Qt official website: https://www.qt.io
- LGPL v3 license: https://www.gnu.org/licenses/lgpl-3.0.html

This software dynamically links to the Qt libraries. No modifications were made to the Qt source code.

## 6. 레퍼런스
https://hangeul.naver.com/fonts/search?f=clova
