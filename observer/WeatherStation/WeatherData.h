#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "Observer.h"
#include <string>

using namespace std;

struct SWeatherInfo
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;

	double minValue = std::numeric_limits<double>::infinity();
	double maxValue = -std::numeric_limits<double>::infinity();
	unsigned countAcc = 0;
	double accValue = 0;
};

class CDisplay: public IObserver<SWeatherInfo>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(SWeatherInfo const& data) override
	{
		std::cout << "------- Current ---------" << std::endl;
		std::cout << "Current Temp " << data.temperature << std::endl;
		std::cout << "Current Hum " << data.humidity << std::endl;
		std::cout << "Current Pressure " << data.pressure << std::endl;
		std::cout << "----------------" << std::endl;
	}
};

class CStatsDisplay : public IObserver<SWeatherInfo>
{
private:
	double GetMax(double const &stats, double &maxValue)
	{
		if (maxValue < stats)
		{
			maxValue = stats;
		}
		return maxValue;
	}
	double GetMin(double const &stats, double &minValue)
	{
		if (minValue > stats)
		{
			minValue = stats;
		}
		return minValue;
	}

	void PrintStatistics(string const &parameterName, double const &accValue,unsigned const &countAcc,double const &maxValue, double const &minValue) const
	{
		std::cout << "------- "<< parameterName <<" ---------" << std::endl;
		std::cout << "Max "<< parameterName << maxValue << std::endl;
		std::cout << "Min " << parameterName << minValue << std::endl;
		std::cout << "Average " << parameterName << (accValue / countAcc) << std::endl;
		std::cout << "----------------" << std::endl;
	}
	void Counters(SWeatherInfo const& data)
	{
		hum.accValue += data.humidity;
		++hum.countAcc;
		press.accValue += data.pressure;
		++press.countAcc;
		temp.accValue += data.temperature;
		++temp.countAcc;
	}
	
	void Update(SWeatherInfo const& data) override
	{
		Counters(data);
		PrintStatistics("Humidity", hum.accValue, hum.countAcc, GetMax(data.humidity, hum.maxValue), GetMin(data.humidity, hum.minValue));
		PrintStatistics("Pressure", press.accValue, press.countAcc, GetMax(data.pressure, press.maxValue), GetMin(data.pressure, press.minValue));
		PrintStatistics("Temperature", temp.accValue , temp.countAcc, GetMax(data.temperature, temp.maxValue), GetMin(data.temperature, temp.minValue));
	}
	SWeatherInfo temp;
	SWeatherInfo press;
	SWeatherInfo hum;
};

class CWeatherData : public CObservable<SWeatherInfo>
{
public:
	double GetTemperature()const
	{
		return m_temperature;
	}
	double GetHumidity()const
	{
		return m_humidity;
	}
	double GetPressure()const
	{
		return m_pressure;
	}

	void MeasurementsChanged()
	{
		NotifyObservers();
	}

	void SetMeasurements(double temp, double humidity, double pressure)
	{
		m_humidity = humidity;
		m_temperature = temp;
		m_pressure = pressure;

		MeasurementsChanged();
	}
protected:
	SWeatherInfo GetChangedData()const override
	{
		SWeatherInfo info;
		info.temperature = GetTemperature();
		info.humidity = GetHumidity();
		info.pressure = GetPressure();
		return info;
	}
private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;	
	double m_pressure = 760.0;
};
