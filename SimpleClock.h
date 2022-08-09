#pragma once
#include<chrono>
#include<unordered_map>
#include<thread>
#include<MMSystem.h>
#pragma comment(lib,"Winmm.lib")
typedef _Return_type_success_(return >= 0) LONG NTSTATUS;
#define STATUS_SUCCESS 0
#define STATUS_TIMER_RESOLUTION_NOT_SET 0xC0000245
static NTSTATUS(NTAPI* NtSetTimerResolution)(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG CurrentResolution) = (NTSTATUS(NTAPI*)(ULONG, BOOLEAN, PULONG)) GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtSetTimerResolution");
using timepoint = std::chrono::steady_clock::time_point;
using millisecond=std::chrono::milliseconds;
using microsecond=std::chrono::microseconds;
class SimpleClock {
private:
	SimpleClock() { 
		m_begin = now(); 
		srand(static_cast<ULONG>(time(0)));
		SetSystemTimerReslutionBegin(0.5f);	
	}
	
	
	SimpleClock(const SimpleClock& _clock) = delete;

	SimpleClock(SimpleClock&&) = delete;

	void operator= (const SimpleClock&) = delete;

	std::unordered_map<std::string,HANDLE> m_sleepEvents;
	
	bool m_bNatieApiOK = true;
	
	ULONG m_CurrentResolution = 0;

	void SetSystemTimerReslutionBegin(float _fResolution) {
		ULONG m_DesiredReslution = static_cast<ULONG>(10000* _fResolution);
		if (NtSetTimerResolution(m_DesiredReslution, TRUE, &m_CurrentResolution) != STATUS_SUCCESS) {
			m_bNatieApiOK = false;
			timeBeginPeriod((int)_fResolution);
		}
	}
	
	void SetSystemTimerReslutionEnd(float _fResolution) {
		if (m_bNatieApiOK) {
			ULONG m_DesiredReslution = static_cast<ULONG>(10000 * _fResolution);
			NtSetTimerResolution(m_DesiredReslution, FALSE, &m_CurrentResolution);
		}else {
			timeEndPeriod((int)_fResolution);
		}
	}
	
	
public:
	virtual ~SimpleClock()noexcept {
		for (auto iter = m_sleepEvents.begin(); iter != m_sleepEvents.end(); iter++) {
			CloseHandle((*iter).second);
		}
		SetSystemTimerReslutionEnd(0.5f);
	};
	timepoint m_begin;

	timepoint m_now;
	
	static SimpleClock& GetInstance(){ static SimpleClock m_instance; return m_instance;}

	timepoint now() {
		m_now= std::chrono::steady_clock::now();
		return m_now;
	}

	float duration(timepoint _begin, timepoint _end) {
		return static_cast<float>(std::chrono::duration_cast<microsecond>(_end - _begin).count());
	}

	float Clock() {return duration(m_begin,now())/1000.0f;}

	float operator()() { return Clock(); }

	void Sleep(int _millisecond) { //一般精度
		std::this_thread::sleep_for(millisecond(_millisecond));
	}

	void SleepEvent(std::string _name,int _millisecond) {
		WaitForSingleObject(GetSleepEvent(_name), _millisecond);
	}

	HANDLE GetSleepEvent(std::string _eventName){
		auto eventiter = m_sleepEvents.find(_eventName);
		if (eventiter != m_sleepEvents.end()) {
			return (*eventiter).second;
		}else {
			HANDLE sleepevt = OpenEventA(EVENT_ALL_ACCESS, FALSE, _eventName.c_str());
			if (sleepevt == NULL)sleepevt = CreateEventA(NULL, TRUE, FALSE, _eventName.c_str());
			m_sleepEvents.insert(std::make_pair(_eventName, sleepevt));
			return sleepevt;
		}
	}

	void CloseSleepEvent(std::string _eventName){
		CloseHandle(m_sleepEvents[_eventName]);
		m_sleepEvents.erase(_eventName);
	}

	timepoint after(timepoint now,float millisecond) {
		return now+ microsecond((LONGLONG)millisecond*1000);
	}
	 
	void Sleep_Until(timepoint _timepoint) {
		if (_timepoint > m_now) {
			std::this_thread::sleep_until(_timepoint);
		}
	}
	void usSleep(DWORD usec) {
		LARGE_INTEGER perfCnt, start, now;
		QueryPerformanceFrequency(&perfCnt);
		QueryPerformanceCounter(&start);
		do {
			QueryPerformanceCounter((LARGE_INTEGER*)&now);
		} while ((now.QuadPart - start.QuadPart) / float(perfCnt.QuadPart) * 1000 * 1000 < usec);
	}
	void HighResolutionSleep(float  _millisecond,std::string name="None") {//睡眠毫秒
		
		auto starttime = std::chrono::high_resolution_clock::now();
		if ((int)_millisecond > 0) {
			WaitForSingleObject(GetSleepEvent(name),(int)_millisecond);
		}
		auto elapsedus = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - starttime).count();
		auto us_diff = (long long)(_millisecond * 1000) - elapsedus;//计算误差 单位us
		if (us_diff > 0&&us_diff<=5000) {
			usSleep(static_cast<DWORD>(us_diff));
		}
	}

};



