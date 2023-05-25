#ifndef BINANCEAPI_H
#define BINANCEAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QUrl>
#include <QUrlQuery>
#include <QMessageAuthenticationCode>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>


class binanceapi : public QObject {
    Q_OBJECT
public:
    explicit binanceapi(const QString& apiKey, const QString& apiSecret, QObject* parent = nullptr);
    void getAccountInformation();
    void ping();
    void getTime();
    void getExchangeInfo();
    void getDepth(const QString& symbol, int limit = 500);
    void getRecentTrades(const QString& symbol, int limit = 500);
    void getHistoricalTrades(const QString& symbol, int limit = 500, qint64 fromId = -1);
    void getAggregateTrades(const QString& symbol, qint64 fromId = -1, qint64 startTime = -1, qint64 endTime = -1, int limit = 500);
    void getKlines(const QString& symbol, const QString& interval, qint64 startTime = -1, qint64 endTime = -1, int limit = 500);
    void getContinuousKlines(const QString& pair, const QString& contractType, const QString& interval, qint64 startTime = -1, qint64 endTime = -1, int limit = 500);
    void getIndexPriceKlines(const QString& pair, const QString& interval, qint64 startTime = -1, qint64 endTime = -1, int limit = 500);
    void getMarkPriceKlines(const QString& symbol, const QString& interval, qint64 startTime = -1, qint64 endTime = -1, int limit = 500);
    void getPremiumIndex(const QString& symbol = "");
    void getFundingRate(const QString& symbol = "", qint64 startTime = -1, qint64 endTime = -1, int limit = 100);
    void get24hrTicker(const QString& symbol = "");
    void getLatestPrice(const QString& symbol = "");
    void getBookTicker(const QString& symbol = "");
    void getOpenInterest(const QString& symbol);
    void getOpenInterestHist(const QString& symbol, const QString& period, qint64 limit = 30, qint64 startTime = -1, qint64 endTime = -1);
    void getTopLongShortAccountRatio(const QString& symbol, const QString& period, qint64 limit = 30, qint64 startTime = -1, qint64 endTime = -1);
    void getTopLongShortPositionRatio(const QString& symbol, const QString& period, qint64 limit = 30, qint64 startTime = -1, qint64 endTime = -1);
    void getGlobalLongShortAccountRatio(const QString& symbol, const QString& period, qint64 limit = 30, qint64 startTime = -1, qint64 endTime = -1);
    void getTakerLongShortRatio(const QString& symbol, const QString& period, qint64 limit = 30, qint64 startTime = -1, qint64 endTime = -1);
    void getLvtKlines(const QString& symbol, const QString& interval, qint64 startTime = -1, qint64 endTime = -1, int limit = 500);
    void getIndexInfo(const QString& symbol = QString());
    void getAssetIndex(const QString& symbol = QString());
    void changePositionMode(bool dualSidePosition, qint64 recvWindow = -1, qint64 timestamp = -1);
    void getPositionMode(qint64 recvWindow = -1, qint64 timestamp = -1);
    void changeMultiAssetsMode(bool multiAssetsMargin, qint64 recvWindow = -1, qint64 timestamp = -1);
    void sendNewOrder(const QString& symbol, const QString& side, const QString& positionSide, const QString& type,
                      const QString& timeInForce, const QString& quantity, const QString& reduceOnly,
                      const QString& price, const QString& newClientOrderId, const QString& stopPrice,
                      const QString& closePosition, const QString& activationPrice, const QString& callbackRate,
                      const QString& workingType, const QString& priceProtect, const QString& newOrderRespType,
                      qint64 recvWindow = -1, qint64 timestamp = -1);
    void modifyOrder(qint64 orderId, const QString& origClientOrderId, const QString& symbol, const QString& side,
                     const QString& quantity, const QString& price, qint64 recvWindow = -1, qint64 timestamp = -1);
    void batchOrders(const QJsonArray& orderList, qint64 recvWindow = -1, qint64 timestamp = -1);
    void batchModifyOrders(const QJsonArray& orderList, qint64 recvWindow = -1, qint64 timestamp = -1);
    void getOrderAmendmentHistory(const QString& symbol, qint64 orderId = -1, const QString& origClientOrderId = QString(), qint64 startTime = -1, qint64 endTime = -1, int limit = 50, qint64 recvWindow = -1, qint64 timestamp = -1);
    void checkOrderStatus(const QString& symbol, qint64 orderId = -1, const QString& origClientOrderId = QString(), qint64 recvWindow = -1, qint64 timestamp = -1);
    void cancelOrder(const QString& symbol, qint64 orderId = -1, const QString& origClientOrderId = QString(), qint64 recvWindow = -1, qint64 timestamp = -1);
    void cancelAllOpenOrders(const QString& symbol, qint64 recvWindow = -1, qint64 timestamp = -1);
    void cancelBatchOrders(const QString& symbol, const QList<qint64>& orderIdList = QList<qint64>(), const QList<QString>& origClientOrderIdList = QList<QString>(), qint64 recvWindow = -1, qint64 timestamp = -1);
    void countdownCancelAll(const QString& symbol, qint64 countdownTime, qint64 recvWindow = -1, qint64 timestamp = -1);
    void getOpenOrder(const QString& symbol, qint64 orderId = -1, const QString& origClientOrderId = "", qint64 recvWindow = -1, qint64 timestamp = -1);
    void getOpenOrders(const QString& symbol = "", qint64 recvWindow = -1, qint64 timestamp = -1);
    void getAllOrders(const QString& symbol, qint64 orderId = -1, qint64 startTime = -1, qint64 endTime = -1, int limit = -1, qint64 recvWindow = -1, qint64 timestamp = -1);
    void getBalance(qint64 recvWindow = -1, qint64 timestamp = -1);
    void getAccountInformation(qint64 recvWindow = -1, qint64 timestamp = -1);
    void changeLeverage(const QString& symbol, int leverage, qint64 recvWindow = -1, qint64 timestamp = -1);
    void changeMarginType(const QString& symbol, const QString& marginType, qint64 recvWindow = -1, qint64 timestamp = -1);
    void adjustPositionMargin(const QString& symbol, const QString& positionSide, double amount, int type, qint64 recvWindow = -1, qint64 timestamp = -1);
    void getPositionMarginHistory(const QString& symbol, int type, qint64 startTime = -1, qint64 endTime = -1, int limit = 500, qint64 recvWindow = -1, qint64 timestamp = -1);
    void getPositionRisk(const QString& symbol = "", qint64 recvWindow = -1, qint64 timestamp = -1);
    void getUserTrades(const QString& symbol, qint64 orderId, qint64 startTime, qint64 endTime, qint64 fromId, int limit, qint64 recvWindow, qint64 timestamp);
    void getIncome(const QString& symbol, const QString& incomeType, qint64 startTime, qint64 endTime, int limit, qint64 recvWindow, qint64 timestamp);
    void getLeverageBracket(const QString& symbol, qint64 recvWindow, qint64 timestamp);
    void getAdlQuantile(const QString& symbol, qint64 recvWindow, qint64 timestamp);
    void getForceOrders(const QString& symbol, const QString& autoCloseType, qint64 startTime, qint64 endTime, int limit, qint64 recvWindow, qint64 timestamp);
    void getApiTradingStatus(const QString& symbol, qint64 recvWindow, qint64 timestamp);
    void getCommissionRate(const QString& symbol, qint64 recvWindow, qint64 timestamp);


    //data stream
    void createUserDataStream();
    void extendUserDataStream(const QString &listenKey);
    void closeUserDataStream(const QString &listenKey);

signals:
    void accountInformationReceived(const QByteArray& data);
    void testConnectivityResultReceived(const QJsonObject& result);
    void snggetdatacandel(QJsonDocument);


private slots:
    void handlePingResponse(QNetworkReply* reply);
    void handleTimeResponse(QNetworkReply* reply);
    void handleExchangeInfoResponse(QNetworkReply* reply);
    void handleDepthResponse(QNetworkReply* reply);
    void handleRecentTradesResponse(QNetworkReply* reply);
    void handleHistoricalTradesResponse(QNetworkReply* reply);
    void handleAggregateTradesResponse(QNetworkReply* reply);
    void handleKlinesResponse(QNetworkReply* reply);
    void handleContinuousKlinesResponse(QNetworkReply* reply);
    void handleIndexPriceKlinesResponse(QNetworkReply* reply);
    void handleMarkPriceKlinesResponse(QNetworkReply* reply);
    void handlePremiumIndexResponse(QNetworkReply* reply);
    void handleFundingRateResponse(QNetworkReply* reply);
    void handle24hrTickerResponse(QNetworkReply* reply);
    void handleLatestPriceResponse(QNetworkReply* reply);
    void handleBookTickerResponse(QNetworkReply* reply);
    void handleOpenInterestResponse(QNetworkReply* reply);
    void handleOpenInterestHistResponse(QNetworkReply* reply);
    void handleTopLongShortAccountRatioResponse(QNetworkReply* reply);
    void handleTopLongShortPositionRatioResponse(QNetworkReply* reply);
    void handleGlobalLongShortAccountRatioResponse(QNetworkReply* reply);
    void handleTakerLongShortRatioResponse(QNetworkReply* reply);
    void handleLvtKlinesResponse(QNetworkReply* reply);
    void handleIndexInfoResponse(QNetworkReply* reply);
    void handleAssetIndexResponse(QNetworkReply* reply);
    void handleChangePositionModeResponse(QNetworkReply* reply);
    void handlePositionModeResponse(QNetworkReply* reply);
    void handleChangeMultiAssetsModeResponse(QNetworkReply* reply);
    void handleNewOrderResponse(QNetworkReply* reply);
    void handleModifyOrderResponse(QNetworkReply* reply);
    void handleBatchOrdersResponse(QNetworkReply* reply);
    void handleBatchModifyOrdersResponse(QNetworkReply* reply);
    void handleOrderAmendmentHistoryResponse(QNetworkReply* reply);
    void handleCheckOrderStatusResponse(QNetworkReply* reply);
    void handleCancelOrderResponse(QNetworkReply* reply);
    void handleCancelAllOpenOrdersResponse(QNetworkReply* reply);
    void handleCancelBatchOrdersResponse(QNetworkReply* reply);
    void handleCountdownCancelAllResponse(QNetworkReply* reply);
    void handleOpenOrderResponse(QNetworkReply* reply);
    void handleOpenOrdersResponse(QNetworkReply* reply);
    void handleAllOrdersResponse(QNetworkReply* reply);
    void handleBalanceResponse(QNetworkReply* reply);
    void handleAccountInformation(QNetworkReply* reply);
    void handleLeverageChange(QNetworkReply* reply);
    void handleMarginTypeChange(QNetworkReply* reply);
    void handlePositionMarginAdjustment(QNetworkReply* reply);
    void handlePositionMarginHistory(QNetworkReply* reply);
    void handlePositionRisk(QNetworkReply* reply);
    void handleUserTrades(QNetworkReply* reply);
    void handleIncome(QNetworkReply* reply);
    void handleLeverageBracket(QNetworkReply* reply);
    void handleAdlQuantile(QNetworkReply* reply);
    void handleForceOrders(QNetworkReply* reply);
    void handleApiTradingStatus(QNetworkReply* reply);
    void handleCommissionRate(QNetworkReply* reply);


    //data stream
    void onCreateUserDataStreamFinished();
    void onExtendUserDataStreamFinished();
    void onCloseUserDataStreamFinished();

private:
    QString generateSignature(const QString& queryString) const;
    QString apiKey;
    QString apiSecret;
    QNetworkAccessManager networkManager;
    QNetworkAccessManager networkManagerstream;

};

#endif // BINANCEAPI_H
