#include "clsShowStockUi.h"
#include <QHBoxLayout>
#include "FinanceChart.h"
#include <QDebug>

// Convert from QDateTime to chartTime
static double QDateTimeToChartTime(QDateTime q)
{
    QDate d = q.date();
    QTime t = q.time();
    return Chart::chartTime(d.year(), d.month(), d.day(), t.hour(), t.minute(),
                            t.second()) + t.msec() / 1000.0;
}


clsShowStockUi::clsShowStockUi(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    initVariables();
    initUi();

    this->showMaximized();
}

clsShowStockUi::~clsShowStockUi()
{
    delete[] m_timeStamps;
    delete[] m_highData;
    delete[] m_lowData;
    delete[] m_openData;
    delete[] m_closeData;
    delete[] m_volData;
    delete[] m_compareData;
}

void clsShowStockUi::setStockCode(QString stockCode)
{
    this->m_TickerSymbol->setText(stockCode);
}

void clsShowStockUi::drawChart()
{
    drawChart(m_ChartViewer);
}

void clsShowStockUi::initVariables()
{
    setWindowTitle(tr("恒通股票参数分析"));

    m_noOfPoints = 0;
    m_timeStamps = 0;
    m_highData = 0;
    m_lowData = 0;
    m_openData = 0;
    m_closeData = 0;
    m_volData = 0;

    m_resolution = 86400;

    m_compareData = 0;
    m_compareDataLen = 0;

    db= new clsDBCreateTables();

}

void clsShowStockUi::initUi()
{
    m_ChartViewer = new QChartViewer();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(m_ChartViewer);
    widget->setLayout(layout);

   // m_ChartViewer->move(8,12);
    m_ChartViewer->setFont(QFont("楷体",14));
    m_TimeRange->addItem(tr("1 个月"), 30);
    m_TimeRange->addItem(tr("2 个月"), 60);
    m_TimeRange->addItem(tr("3 个月"), 90);
    m_TimeRange->addItem(tr("6 个月"), 180);
    m_TimeRange->addItem(tr("1 年"), 360);
    m_TimeRange->addItem(tr("2 年"), 720);
    m_TimeRange->addItem(tr("3 年"), 1080);
    m_TimeRange->addItem(tr("4 年"), 1440);
    m_TimeRange->addItem(tr("5 年"), 1800);
    m_TimeRange->addItem(tr("10 年"), 3600);
    m_TimeRange->setCurrentIndex(3);

    m_ChartType->addItem("None", "None");
    m_ChartType->addItem("蜡烛图", "CandleStick");
    m_ChartType->addItem("收盘价", "Close");
    m_ChartType->addItem("中间价", "Median");
    m_ChartType->addItem("OHLC", "OHLC");
    m_ChartType->addItem("典型价格", "TP");
    m_ChartType->addItem("加权收盘价", "WC");
    m_ChartType->setCurrentIndex(1);

    m_PriceBand->addItem("None", "None");
    m_PriceBand->addItem("布林线指标", "BB");
    m_PriceBand->addItem("Donchain通道指标", "DC");
    m_PriceBand->addItem("包络线指标 (SMA 20 +/- 10%)", "Envelop");
    m_PriceBand->setCurrentIndex(1);

    m_AvgType1->addItem("None", "None");
    m_AvgType1->addItem("Simple", "SMA");
    m_AvgType1->addItem("Exponential", "EMA");
    m_AvgType1->addItem("Triangular", "TMA");
    m_AvgType1->addItem("Weighted", "WMA");
    m_AvgType1->setCurrentIndex(1);

    for (int i = 0; i < m_AvgType1->count(); ++i)
        m_AvgType2->addItem(m_AvgType1->itemText(i), m_AvgType1->itemData(i));
    m_AvgType2->setCurrentIndex(1);


    m_Indicator1->addItem("None", "None");
    m_Indicator1->addItem("Accumulation/Distribution", "AccDist");
    m_Indicator1->addItem("Aroon Oscillator", "AroonOsc");
    m_Indicator1->addItem("Aroon Up/Down", "Aroon");
    m_Indicator1->addItem("Avg Directional Index", "ADX");
    m_Indicator1->addItem("Avg True Range", "ATR");
    m_Indicator1->addItem("Bollinger Band Width", "BBW");
    m_Indicator1->addItem("Chaikin Money Flow", "CMF");
    m_Indicator1->addItem("Chaikin Oscillator", "COscillator");
    m_Indicator1->addItem("Chaikin Volatility", "CVolatility");
    m_Indicator1->addItem("Close Location Value", "CLV");
    m_Indicator1->addItem("Commodity Channel Index", "CCI");
    m_Indicator1->addItem("Detrended Price Osc", "DPO");
    m_Indicator1->addItem("Donchian Channel Width", "DCW");
    m_Indicator1->addItem("Ease of Movement", "EMV");
    m_Indicator1->addItem("Fast Stochastic", "FStoch");
    m_Indicator1->addItem("MACD", "MACD");
    m_Indicator1->addItem("Mass Index", "MDX");
    m_Indicator1->addItem("Momentum", "Momentum");
    m_Indicator1->addItem("Money Flow Index", "MFI");
    m_Indicator1->addItem("Neg Volume Index", "NVI");
    m_Indicator1->addItem("On Balance Volume", "OBV");
    m_Indicator1->addItem("Performance", "Performance");
    m_Indicator1->addItem("% Price Oscillator", "PPO");
    m_Indicator1->addItem("% Volume Oscillator", "PVO");
    m_Indicator1->addItem("Pos Volume Index", "PVI");
    m_Indicator1->addItem("Price Volume Trend", "PVT");
    m_Indicator1->addItem("Rate of Change", "ROC");
    m_Indicator1->addItem("RSI", "RSI");
    m_Indicator1->addItem("Slow Stochastic", "SStoch");
    m_Indicator1->addItem("StochRSI", "StochRSI");
    m_Indicator1->addItem("TRIX", "TRIX");
    m_Indicator1->addItem("Ultimate Oscillator", "UO");
    m_Indicator1->addItem("Volume", "Vol");
    m_Indicator1->addItem("William's %R", "WilliamR");

    for (int i = 0; i < m_Indicator1->count(); ++i)
    {
        m_Indicator2->addItem(m_Indicator1->itemText(i), m_Indicator1->itemData(i));
        //m_Indicator3->addItem(m_Indicator1->itemText(i), m_Indicator1->itemData(i));
       // m_Indicator4->addItem(m_Indicator1->itemText(i), m_Indicator1->itemData(i));
    }

    m_Indicator1->setCurrentIndex(m_Indicator1->findData("RSI"));
    m_Indicator2->setCurrentIndex(m_Indicator2->findData("MACD"));
   // m_Indicator3->setCurrentIndex(0);
   // m_Indicator4->setCurrentIndex(0);

    const QObjectList &allControls = this->children();
    for (int i = 0; i < allControls.count(); ++i)
    {
        QObject *obj;
        if ((obj = qobject_cast<QComboBox *>(allControls[i])) != 0)
            connect(obj, SIGNAL(currentIndexChanged(int)), SLOT(onComboBoxChanged(int)));
        else if ((obj = qobject_cast<QCheckBox *>(allControls[i])) != 0)
            connect(obj, SIGNAL(clicked()), SLOT(onCheckBoxChanged()));
        else if ((obj = qobject_cast<QLineEdit *>(allControls[i])) != 0)
            connect(obj, SIGNAL(editingFinished()), SLOT(onLineEditChanged()));
    }
}


static LineLayer* addMovingAvg(FinanceChart *m, QString avgType, int avgPeriod, int color)
{
    if (avgPeriod > 1)
    {
        if (avgType == "SMA")
            return m->addSimpleMovingAvg(avgPeriod, color);
        else if (avgType == "EMA")
            return m->addExpMovingAvg(avgPeriod, color);
        else if (avgType == "TMA")
            return m->addTriMovingAvg(avgPeriod, color);
        else if (avgType == "WMA")
            return m->addWeightedMovingAvg(avgPeriod, color);
    }

    return 0;
}

static void errMsg(QChartViewer* viewer, const char *msg)
{
    MultiChart m(400, 200);
    m.addTitle(Chart::Center, msg, "Arial", 10)->setMaxWidth(m.getWidth());
    viewer->setChart(&m);
}


static XYChart* addIndicator(FinanceChart *m, QString indicator, int height)
{
    if (indicator == "RSI")
        return m->addRSI(height, 14, 0x800080, 20, 0xff6666, 0x6666ff);
    else if (indicator == "StochRSI")
        return m->addStochRSI(height, 14, 0x800080, 30, 0xff6666, 0x6666ff);
    else if (indicator == "MACD")
        return m->addMACD(height, 26, 12, 9, 0xff, 0xff00ff, 0x8000);
    else if (indicator == "FStoch")
        return m->addFastStochastic(height, 14, 3, 0x6060, 0x606000);
    else if (indicator == "SStoch")
        return m->addSlowStochastic(height, 14, 3, 0x6060, 0x606000);
    else if (indicator == "ATR")
        return m->addATR(height, 14, 0x808080, 0xff);
    else if (indicator == "ADX")
        return m->addADX(height, 14, 0x8000, 0x800000, 0x80);
    else if (indicator == "DCW")
        return m->addDonchianWidth(height, 20, 0xff);
    else if (indicator == "BBW")
        return m->addBollingerWidth(height, 20, 2, 0xff);
    else if (indicator == "DPO")
        return m->addDPO(height, 20, 0xff);
    else if (indicator == "PVT")
        return m->addPVT(height, 0xff);
    else if (indicator == "Momentum")
        return m->addMomentum(height, 12, 0xff);
    else if (indicator == "Performance")
        return m->addPerformance(height, 0xff);
    else if (indicator == "ROC")
        return m->addROC(height, 12, 0xff);
    else if (indicator == "OBV")
        return m->addOBV(height, 0xff);
    else if (indicator == "AccDist")
        return m->addAccDist(height, 0xff);
    else if (indicator == "CLV")
        return m->addCLV(height, 0xff);
    else if (indicator == "WilliamR")
        return m->addWilliamR(height, 14, 0x800080, 30, 0xff6666, 0x6666ff);
    else if (indicator == "Aroon")
        return m->addAroon(height, 14, 0x339933, 0x333399);
    else if (indicator == "AroonOsc")
        return m->addAroonOsc(height, 14, 0xff);
    else if (indicator == "CCI")
        return m->addCCI(height, 20, 0x800080, 100, 0xff6666, 0x6666ff);
    else if (indicator == "EMV")
        return m->addEaseOfMovement(height, 9, 0x6060, 0x606000);
    else if (indicator == "MDX")
        return m->addMassIndex(height, 0x800080, 0xff6666, 0x6666ff);
    else if (indicator == "CVolatility")
        return m->addChaikinVolatility(height, 10, 10, 0xff);
    else if (indicator == "COscillator")
        return m->addChaikinOscillator(height, 0xff);
    else if (indicator == "CMF")
        return m->addChaikinMoneyFlow(height, 21, 0x8000);
    else if (indicator == "NVI")
        return m->addNVI(height, 255, 0xff, 0x883333);
    else if (indicator == "PVI")
        return m->addPVI(height, 255, 0xff, 0x883333);
    else if (indicator == "MFI")
        return m->addMFI(height, 14, 0x800080, 30, 0xff6666, 0x6666ff);
    else if (indicator == "PVO")
        return m->addPVO(height, 26, 12, 9, 0xff, 0xff00ff, 0x8000);
    else if (indicator == "PPO")
        return m->addPPO(height, 26, 12, 9, 0xff, 0xff00ff, 0x8000);
    else if (indicator == "UO")
        return m->addUltimateOscillator(height, 7, 14, 28, 0x800080, 20, 0xff6666, 0x6666ff);
    else if (indicator == "Vol")
        return m->addVolIndicator(height, 0x99ff99, 0xff9999, 0xc0c0c0);
    else if (indicator == "TRIX")
        return m->addTRIX(height, 12, 0xff);
    else
        return 0;
}


void clsShowStockUi::drawChart(QChartViewer *viewer)
{
    res = db->getStockData(this->m_TickerSymbol->text());

    if(res.isEmpty())
        return;



    // In this demo, we just assume we plot up to the latest time. So endDate is now.
    QDateTime endDate = QDateTime(res.first().getDate(),QTime(16,0,0));//QDateTime::currentDateTime();

    // If the trading day has not yet started (before 9:30am), or if the end date is on
    // on Sat or Sun, we set the end date to 4:00pm of the last trading day
    while ((endDate.time() < QTime(9, 30, 0)) || (endDate.date().dayOfWeek() >= 6)) {
        endDate = QDateTime(endDate.date().addDays(-1), QTime(16, 0, 0));
    }

    // The duration selected by the user
    int durationInDays = m_TimeRange->itemData(m_TimeRange->currentIndex()).toInt();

    // Compute the start date by subtracting the duration from the end date.
    QDateTime startDate = endDate;
    if (durationInDays >= 30) {
        // More or equal to 30 days - so we use months as the unit
        startDate = QDateTime(QDate(endDate.date().year(), endDate.date().month(), 1
                                    ).addMonths(-durationInDays / 30));
    } else {
        // Less than 30 days - use day as the unit. The starting point of the axis is
        // always at the start of the day (9:30am). Note that we use trading days, so
        // we skip Sat and Sun in counting the days.
        startDate = QDateTime(endDate.date());
        for(int i = 1; i < durationInDays; ++i) {
            if (startDate.date().dayOfWeek() == 1) {
                startDate = startDate.addDays(-3);
            } else {
                startDate = startDate.addDays(-1);
            }
        }
    }

    // The first moving average period selected by the user.
    m_avgPeriod1 = m_MovAvg1->text().toInt();
    if (m_avgPeriod1 < 0)
        m_avgPeriod1 = 0;
    if (m_avgPeriod1 > 300)
        m_avgPeriod1 = 300;

    // The second moving average period selected by the user.
    m_avgPeriod2 = m_MovAvg2->text().toInt();
    if (m_avgPeriod2 < 0)
        m_avgPeriod2 = 0;
    if (m_avgPeriod2 > 300)
        m_avgPeriod2 = 300;

    // We need extra leading data points in order to compute moving averages.
    int extraPoints = (m_avgPeriod1 > m_avgPeriod2) ? m_avgPeriod1 : m_avgPeriod2;
    if (extraPoints < 25)
        extraPoints = 25;


    // The data series we want to get.
    m_tickerKey = m_TickerSymbol->text();
    if (!getData(m_tickerKey, startDate, endDate, durationInDays, extraPoints))
    {
        errMsg(viewer, "Please enter a valid ticker symbol");
        return;
    }

    // We now confirm the actual number of extra points (data points that are before
    // the start date) as inferred using actual data from the database.
    double startChartTime = QDateTimeToChartTime(startDate);
    for (extraPoints = 0; extraPoints < m_noOfPoints; ++extraPoints)
    {
        if (m_timeStamps[extraPoints] >= startChartTime)
            break;
    }

    // Check if there is any valid data
    if (extraPoints >= m_noOfPoints)
    {
        // No data - just display the no data message.
        errMsg(viewer, "No data available for the specified time period");
        return;
    }

    // In some finance chart presentation style, even if the data for the latest day
    // is not fully available, the axis for the entire day will still be drawn, where
    // no data will appear near the end of the axis.
    int extraTrailingPoints = 0;
    if (m_resolution <= 86400)
    {
        // Add extra points to the axis until it reaches the end of the day. The end
        // of day is assumed to be 16:00 (it depends on the stock exchange).
        double lastTime = m_timeStamps[m_noOfPoints - 1];
        extraTrailingPoints = (int)((16 * 3600 - fmod(lastTime, 86400)) / m_resolution);
        if (extraTrailingPoints > 0)
        {
            double *extendedTimeStamps = new double[m_noOfPoints + extraTrailingPoints];
            memcpy(extendedTimeStamps, m_timeStamps, sizeof(double) * m_noOfPoints);
            for (int i = 0; i < extraTrailingPoints; ++i)
                extendedTimeStamps[m_noOfPoints + i] = lastTime + m_resolution * (i + 1);
            delete[] m_timeStamps;
            m_timeStamps = extendedTimeStamps;
        }
    }

    //
    // At this stage, all data is available. We can draw the chart as according to
    // user input.
    //

    //
    // Determine the chart size. In this demo, user can select 4 different chart sizes.
    // Default is the large chart size.
    //

    int width = widget->width()-16;
    int mainHeight = widget->height()-240-14-55;
    int indicatorHeight = 120;

    // Create the chart object using the selected size
    FinanceChart m(width);

    // Set the data into the chart object
    m.setData(DoubleArray(m_timeStamps, m_noOfPoints + extraTrailingPoints),
              DoubleArray(m_highData, m_noOfPoints), DoubleArray(m_lowData, m_noOfPoints),
              DoubleArray(m_openData, m_noOfPoints), DoubleArray(m_closeData, m_noOfPoints),
              DoubleArray(m_volData, m_noOfPoints), extraPoints);


    //
    // We configure the title of the chart. In this demo chart design, we put the
    // company name as the top line of the title with left alignment.
    //
    m.addPlotAreaTitle(Chart::TopLeft, m_tickerKey.toUtf8().data());

    // We displays the current date as well as the data resolution on the next line.
    const char *resolutionText = "";
    if (m_resolution == 30 * 86400)
        resolutionText = "Monthly";
    else if (m_resolution == 7 * 86400)
        resolutionText = "Weekly";
    else if (m_resolution == 86400)
        resolutionText = "Daily";
    else if (m_resolution == 900)
        resolutionText = "15-min";

    char buffer[1024];
    sprintf(buffer, "<*font=arial.ttf,size=8*>%s - %s chart", m.formatValue(
                QDateTimeToChartTime(QDateTime::currentDateTime()), "mmm dd, yyyy"), resolutionText);
    m.addPlotAreaTitle(Chart::BottomLeft, buffer);

    // A copyright message at the bottom left corner the title area
    m.addPlotAreaTitle(Chart::BottomRight,
                       "<*font=arial.ttf,size=8*>Abama Stock Analysis");

    //
    // Add the first techical indicator according. In this demo, we draw the first
    // indicator on top of the main chart.
    //
    addIndicator(&m, m_Indicator1->itemData(m_Indicator1->currentIndex()).toString(),
                 indicatorHeight);

    //
    // Add the main chart
    //
    m.addMainChart(mainHeight);

    //
    // Set log or linear scale according to user preference
    //
    m.setLogScale(m_LogScale->isChecked());

    //
    // Set axis labels to show data values or percentage change to user preference
    //
    if (m_PercentageScale->isChecked())
        m.setPercentageAxis();

    //
    // Draw the main chart depending on the chart type the user has selected
    //
    QString selectedType = m_ChartType->itemData(m_ChartType->currentIndex()).toString();
    if (selectedType == "Close")
        m.addCloseLine(0x000040);
    else if (selectedType == "TP")
        m.addTypicalPrice(0x000040);
    else if (selectedType == "WC")
        m.addWeightedClose(0x000040);
    else if (selectedType == "Median")
        m.addMedianPrice(0x000040);

    //
    // Add comparison line if there is data for comparison
    //
    if (m_compareData != 0) {
        if (m_compareDataLen > extraPoints) {
            m.addComparison(DoubleArray(m_compareData, m_compareDataLen), 0x0000ff,
                            m_compareKey.toUtf8().data());
        }
    }

    //
    // Add moving average lines.
    //
    addMovingAvg(&m, m_AvgType1->itemData(m_AvgType1->currentIndex()).toString(),
                 m_avgPeriod1, 0x663300);
    addMovingAvg(&m, m_AvgType2->itemData(m_AvgType2->currentIndex()).toString(),
                 m_avgPeriod2, 0x9900ff);

    //
    // Draw the main chart if the user has selected CandleStick or OHLC. We
    // draw it here to make sure it is drawn behind the moving average lines
    // (that is, the moving average lines stay on top.)
    //
    if (selectedType == "CandleStick")
        m.addCandleStick(0xff3333,0x33ff33);
    else if (selectedType == "OHLC")
        m.addHLOC(0x800000,0x8000);

    //
    // Add parabolic SAR if necessary
    //
    if (m_ParabolicSAR->isChecked())
        m.addParabolicSAR(0.02, 0.02, 0.2, Chart::DiamondShape, 5, 0x008800, 0x000000);

    //
    // Add price band/channel/envelop to the chart according to user selection
    //
    QString selectedBand = m_PriceBand->itemData(m_PriceBand->currentIndex()).toString();
    if (selectedBand == "BB")
        m.addBollingerBand(20, 2, 0x9999ff, 0xc06666ff);
    else if (selectedBand == "DC")
        m.addDonchianChannel(20, 0x9999ff, 0xc06666ff);
    else if (selectedBand == "Envelop")
        m.addEnvelop(20, 0.1, 0x9999ff, 0xc06666ff);

    //
    // Add volume bars to the main chart if necessary
    //
    if (m_VolumeBars->isChecked())
        m.addVolBars(indicatorHeight, 0xff9999,0x99ff99,  0xc0c0c0);

    //
    // Add additional indicators as according to user selection.
    //
    addIndicator(&m, m_Indicator2->itemData(m_Indicator2->currentIndex()).toString(),
                 indicatorHeight);
//    addIndicator(&m, m_Indicator3->itemData(m_Indicator3->currentIndex()).toString(),
//                 indicatorHeight);
//    addIndicator(&m, m_Indicator4->itemData(m_Indicator4->currentIndex()).toString(),
//                 indicatorHeight);

    // Set the chart to the viewer
    viewer->setChart(&m);

    // Set image map (for tool tips) to the viewer
    sprintf(buffer, "title='%s {value|P}'", m.getToolTipDateFormat());
    viewer->setImageMap(m.getHTMLImageMap("", "", buffer));
}

void clsShowStockUi::resizeEvent(QResizeEvent */*e*/)
{


    drawChart(this->m_ChartViewer);
}


void clsShowStockUi::aggregateData(ArrayMath &aggregator)
{
    aggregator.aggregate(DoubleArray(m_highData, m_noOfPoints), Chart::AggregateMax);
    aggregator.aggregate(DoubleArray(m_lowData, m_noOfPoints), Chart::AggregateMin);
    aggregator.aggregate(DoubleArray(m_openData, m_noOfPoints), Chart::AggregateFirst);
    aggregator.aggregate(DoubleArray(m_closeData, m_noOfPoints), Chart::AggregateLast);
    aggregator.aggregate(DoubleArray(m_volData, m_noOfPoints), Chart::AggregateSum);
    m_noOfPoints = aggregator.aggregate(DoubleArray(m_timeStamps, m_noOfPoints),
                                        Chart::AggregateFirst).len;
}

void clsShowStockUi::convertDailyToMonthlyData()
{
    aggregateData(ArrayMath(DoubleArray(m_timeStamps, m_noOfPoints)).selectStartOfMonth());
}

void clsShowStockUi::convertDailyToWeeklyData()
{
    aggregateData(ArrayMath(DoubleArray(m_timeStamps, m_noOfPoints)).selectStartOfWeek());
}

void clsShowStockUi::onComboBoxChanged(int)
{
    drawChart(m_ChartViewer);
}

void clsShowStockUi::onCheckBoxChanged()
{
    drawChart(m_ChartViewer);
}

void clsShowStockUi::onLineEditChanged()
{
    int new_avgPeriod1 = m_MovAvg1->text().toInt();
    int new_avgPeriod2 = m_MovAvg2->text().toInt();

    QString tickerKey = m_TickerSymbol->text();
    //    QString compareKey = m_CompareWith->text();

    if ((new_avgPeriod1 != m_avgPeriod1) || (new_avgPeriod2 != m_avgPeriod2) ||
            (m_tickerKey != tickerKey) )/*|| (m_compareKey != compareKey))*/
        drawChart(m_ChartViewer);
}



bool clsShowStockUi::getData(const QString &ticker, QDateTime startDate, QDateTime endDate,
                             int durationInDays, int extraPoints)
{

    if (ticker.isEmpty())
        return false;


    m_resolution = 86400;
    if (durationInDays >= 2.0 * 360)
    {
        // 4 years or more - use monthly data points.
        m_resolution = 7 * 86400;

        // Adjust startDate backwards to cater for extraPoints
        QDateTime adjustedStartDate(startDate.date().addMonths(-extraPoints));

        // Get the required monthly data
        getMonthlyData(ticker, adjustedStartDate, endDate);
    }
    else if (durationInDays >= 1.0 * 360)
    {
        // 1 year or more - use weekly points.
        m_resolution = 7 * 86400;

        //Note that we need to add extra points by shifting the starting weeks backwards
        QDateTime adjustedStartDate(startDate.date().addDays(-extraPoints * 7 - 6));

        // Get the required weekly data
        getWeeklyData(ticker, adjustedStartDate, endDate);
    }
    else
    {
        // Default - use daily points
        m_resolution = 86400;

        // Adjust startDate backwards to cater for extraPoints. We multiply the days
        // by 7/5 as we assume 1 week has 5 trading days.
        QDateTime adjustedStartDate(startDate.date().addDays(-((extraPoints * 7 + 4) / 4 + 2)));

        // Get the required daily data
        getDailyData(ticker, adjustedStartDate, endDate);
    }

    return true;
}


void clsShowStockUi::getDailyData(const QString &ticker, QDateTime startDate, QDateTime /*endDate*/)
{

    if(ticker.isEmpty())
        return;

    int i=0;

    while(startDate <= QDateTime(res[i].getDate()))
    {
        i++;
        if(i>= res.length())
        {
            i=i-1;
            break;
        }
    }

    // free the previous data arrays
    delete[] m_timeStamps;
    delete[] m_highData;
    delete[] m_lowData;
    delete[] m_openData;
    delete[] m_closeData;
    delete[] m_volData;

    // Allocate the data arrays
    m_noOfPoints =i;
    m_timeStamps = new double[i];
    m_highData = new double[i];
    m_lowData = new double[i];
    m_openData = new double[i];
    m_closeData = new double[i];
    m_volData = new double[i];

    for(int j=0;j<i;j++)
    {
        m_timeStamps[j]= QDateTimeToChartTime(QDateTime(res[i-j-1].getDate()));
        m_highData[j] = res.at(i-j-1).high;
        m_lowData[j] = res.at(i-j-1).low;
        m_openData[j] = res.at(i-j-1).open;
        m_closeData[j] = res.at(i-j-1).close;
        m_volData[j] =  res.at(i-j-1).cjl;

    }
}

void clsShowStockUi::getWeeklyData(const QString &ticker, QDateTime startDate, QDateTime endDate)
{
    getDailyData(ticker, startDate, endDate);
    convertDailyToWeeklyData();
}


void clsShowStockUi::getMonthlyData(const QString &ticker, QDateTime startDate, QDateTime endDate)
{

    getDailyData(ticker, startDate, endDate);
    convertDailyToMonthlyData();

}
