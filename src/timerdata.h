#pragma once
#include "timesectostring.h"
#include <QDateTime>

namespace NS_Timer
{

// This class is a stores of all timer data in current session
// Used for sends to database for record in table
class TimerData
{
public:
	TimerData()																= default;
	TimerData(const TimerData&)										= delete;
	TimerData(TimerData&&)											= delete;
	virtual ~TimerData()													= default;

	TimerData&		operator=(const TimerData&)					= delete;
	TimerData&		operator=(TimerData&&)						= delete;

	void				Reset();

	// Set data from timer
	void				WorkSeconds(uint32_t sec);
	void				WritingCodeSeconds(uint32_t sec);
	void				EndTime(const QTime&);

	// Get data for the database
	QDate			DateStart()								const;
	QString			TimeStart()								const;
	QString			TimeEnd()								const;
	QString			TimeWork()								const;
	QString			TimeWritingCode()					const;
	int					SecondsWork()						const noexcept;
	int					SecondsWritingCode()				const noexcept;

private:
	int					workSec_				= 0;
	int					writingCodeSec_		= 0;
	QDateTime		startDateTime_;
	QTime			endTime_;
};

}//namespace NS_Timer
