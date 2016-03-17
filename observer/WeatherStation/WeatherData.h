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
		m_accValueForHum += data.humidity;
		++m_countAccForHum;
		m_accValueForPress += data.pressure;
		++m_countAccForPress;
		m_accValueForTemp += data.temperature;
		++m_countAccForTemp;
	}
	void Update(SWeatherInfo const& data) override
	{
		Counters(data);
		PrintStatistics("Humidity", m_accValueForHum, m_countAccForHum, GetMax(data.humidity, m_maxValueForHum), GetMin(data.humidity, m_minValueForHum));
		PrintStatistics("Pressure", m_accValueForPress, m_countAccForPress, GetMax(data.pressure, m_maxValueForPress), GetMin(data.pressure, m_minValueForPress));
		PrintStatistics("Temperature", m_accValueForTemp, m_countAccForTemp, GetMax(data.temperature, m_maxValueForTemp), GetMin(data.temperature, m_minValueForTemp));
	}

	double m_minValueForTemp = std::numeric_limits<double>::infinity();
	double m_minValueForPress = std::numeric_limits<double>::infinity();
	double m_minValueForHum = std::numeric_limits<double>::infinity();

	double m_maxValueForTemp = -std::numeric_limits<double>::infinity();
	double m_maxValueForPress = -std::numeric_limits<double>::infinity();
	double m_maxValueForHum = -std::numeric_limits<double>::infinity();

	double m_accValueForTemp = 0;
	double m_accValueForPress = 0;
	double m_accValueForHum = 0;

	unsigned m_countAccForTemp = 0;
	unsigned m_countAccForPress = 0;
	unsigned m_countAccForHum = 0;

};

class CWeatherData : public CObservable<SWeatherInfo>
{
public:
	// Температура в градусах Цельсия
	double GetTemperature()const
	{
		return m_temperature;
	}
	// Относительная влажность (0...100)
	double GetHumidity()const
	{
		return m_humidity;
	}
	// Атмосферное давление (в мм.рт.ст)
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
