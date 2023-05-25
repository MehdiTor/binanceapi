/*
======================================================================
=        Copyright (C) Mehdi torshani  email:mehdi.torshani@gmail.com
=                       .:M.T:.
======================================================================
*/
#include "binanceapi.h"


binanceapi::binanceapi(const QString& apiKey, const QString& apiSecret, QObject* parent)
    : QObject(parent), apiKey(apiKey), apiSecret(apiSecret) {
}

void binanceapi::getAccountInformation() {
    QUrl url("https://api.binance.com/api/v3/account");
    QUrlQuery query;
    query.addQueryItem("timestamp", QString::number(QDateTime::currentMSecsSinceEpoch()));
    QString signature = generateSignature(query.toString());
    query.addQueryItem("signature", signature);
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleAccountInformation(reply); });
}


QString binanceapi::generateSignature(const QString& queryString) const {
    return QMessageAuthenticationCode::hash(queryString.toUtf8(), apiSecret.toUtf8(), QCryptographicHash::Sha256).toHex();
}

void binanceapi::ping() {
    QUrl url("https://fapi.binance.com/fapi/v1/ping");
    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handlePingResponse(reply); });
}

void binanceapi::handlePingResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        qDebug() << "Ping successful!";
    }
    reply->deleteLater();
}
void binanceapi::getTime() {
    QUrl url("https://fapi.binance.com/fapi/v1/time");
    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleTimeResponse(reply); });
}

void binanceapi::handleTimeResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        if (jsonObject.contains("serverTime")) {
            qDebug() << "Server time:" << jsonObject["serverTime"].toString();
        }
    }
    reply->deleteLater();
}

void binanceapi::getExchangeInfo() {
    QUrl url("https://fapi.binance.com/fapi/v1/exchangeInfo");
    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleExchangeInfoResponse(reply); });
}

void binanceapi::handleExchangeInfoResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Exchange Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getDepth(const QString& symbol, int limit) {
    if (limit != 5 && limit != 10 && limit != 20 && limit != 50 &&
        limit != 100 && limit != 500 && limit != 1000) {
        qDebug() << "Invalid limit. Valid limits are: 5, 10, 20, 50, 100, 500, 1000";
        return;
    }

    QUrl url("https://fapi.binance.com/fapi/v1/depth");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("limit", QString::number(limit));
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleDepthResponse(reply); });
}

void binanceapi::handleDepthResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Depth Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getRecentTrades(const QString& symbol, int limit) {
    if (limit < 1 || limit > 1000) {
        qDebug() << "Invalid limit. Valid limits are between 1 and 1000";
        return;
    }

    QUrl url("https://fapi.binance.com/fapi/v1/trades");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("limit", QString::number(limit));
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleRecentTradesResponse(reply); });
}

void binanceapi::handleRecentTradesResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Recent Trades Info:" << jsonObject;
    }
    reply->deleteLater();
}

void binanceapi::getHistoricalTrades(const QString& symbol, int limit, qint64 fromId) {
    if (limit < 1 || limit > 1000) {
        qDebug() << "Invalid limit. Valid limits are between 1 and 1000";
        return;
    }

    QUrl url("https://fapi.binance.com/fapi/v1/historicalTrades");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("limit", QString::number(limit));
    if (fromId >= 0) {
        query.addQueryItem("fromId", QString::number(fromId));
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleHistoricalTradesResponse(reply); });
}

void binanceapi::handleHistoricalTradesResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Historical Trades Info:" << jsonObject;
    }
    reply->deleteLater();
}

void binanceapi::getAggregateTrades(const QString& symbol, qint64 fromId, qint64 startTime, qint64 endTime, int limit) {
    if (limit < 1 || limit > 1000) {
        qDebug() << "Invalid limit. Valid limits are between 1 and 1000";
        return;
    }

    QUrl url("https://fapi.binance.com/fapi/v1/aggTrades");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("limit", QString::number(limit));

    if (fromId >= 0) {
        query.addQueryItem("fromId", QString::number(fromId));
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleAggregateTradesResponse(reply); });
}

void binanceapi::handleAggregateTradesResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Aggregate Trades Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getKlines(const QString& symbol, const QString& interval, qint64 startTime, qint64 endTime, int limit) {
    if (limit < 1 || limit > 1500) {
        qDebug() << "Invalid limit. Valid limits are between 1 and 1500";
        return;
    }

    QUrl url("https://fapi.binance.com/fapi/v1/klines");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("interval", interval);
    query.addQueryItem("limit", QString::number(limit));

    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleKlinesResponse(reply); });
}

void binanceapi::handleKlinesResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        if(!jsonResponse.isNull())
        {
                emit snggetdatacandel(jsonResponse);
        }
    }
    reply->deleteLater();

}
void binanceapi::checkOrderStatus(const QString& symbol, qint64 orderId, const QString& origClientOrderId, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/order");
    QUrlQuery query;

    query.addQueryItem("symbol", symbol);
    if (orderId >= 0) {
        query.addQueryItem("orderId", QString::number(orderId));
    }
    if (!origClientOrderId.isEmpty()) {
        query.addQueryItem("origClientOrderId", origClientOrderId);
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleCheckOrderStatusResponse(reply); });
}

void binanceapi::handleCheckOrderStatusResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Check Order Status Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getContinuousKlines(const QString& pair, const QString& contractType, const QString& interval, qint64 startTime, qint64 endTime, int limit) {
    if (limit < 1 || limit > 1500) {
        qDebug() << "Invalid limit. Valid limits are between 1 and 1500";
        return;
    }

    QUrl url("https://fapi.binance.com/fapi/v1/continuousKlines");
    QUrlQuery query;
    query.addQueryItem("pair", pair);
    query.addQueryItem("contractType", contractType);
    query.addQueryItem("interval", interval);
    query.addQueryItem("limit", QString::number(limit));

    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleContinuousKlinesResponse(reply); });
}

void binanceapi::handleContinuousKlinesResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Continuous Klines Info:" << jsonObject;
    }
    reply->deleteLater();
}

void binanceapi::getIndexPriceKlines(const QString& pair, const QString& interval, qint64 startTime, qint64 endTime, int limit) {
    if (limit < 1 || limit > 1500) {
        qDebug() << "Invalid limit. Valid limits are between 1 and 1500";
        return;
    }

    QUrl url("https://fapi.binance.com/fapi/v1/indexPriceKlines");
    QUrlQuery query;
    query.addQueryItem("pair", pair);
    query.addQueryItem("interval", interval);
    query.addQueryItem("limit", QString::number(limit));

    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleIndexPriceKlinesResponse(reply); });
}

void binanceapi::handleIndexPriceKlinesResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Index Price Klines Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getMarkPriceKlines(const QString& symbol, const QString& interval, qint64 startTime, qint64 endTime, int limit) {
    if (limit < 1 || limit > 1500) {
        qDebug() << "Invalid limit. Valid limits are between 1 and 1500";
        return;
    }

    QUrl url("https://fapi.binance.com/fapi/v1/markPriceKlines");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("interval", interval);
    query.addQueryItem("limit", QString::number(limit));

    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleMarkPriceKlinesResponse(reply); });
}

void binanceapi::handleMarkPriceKlinesResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Mark Price Klines Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getPremiumIndex(const QString& symbol) {
    QUrl url("https://fapi.binance.com/fapi/v1/premiumIndex");
    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handlePremiumIndexResponse(reply); });
}

void binanceapi::handlePremiumIndexResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Premium Index Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getFundingRate(const QString& symbol, qint64 startTime, qint64 endTime, int limit) {
    QUrl url("https://fapi.binance.com/fapi/v1/fundingRate");
    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    if (limit > 0 && limit <= 1000) {
        query.addQueryItem("limit", QString::number(limit));
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleFundingRateResponse(reply); });
}

void binanceapi::handleFundingRateResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Funding Rate Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::get24hrTicker(const QString& symbol) {
    QUrl url("https://fapi.binance.com/fapi/v1/ticker/24hr");
    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handle24hrTickerResponse(reply); });
}

void binanceapi::handle24hrTickerResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "24hr Ticker Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getLatestPrice(const QString& symbol) {
    QUrl url("https://fapi.binance.com/fapi/v1/ticker/price");
    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleLatestPriceResponse(reply); });
}

void binanceapi::handleLatestPriceResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Latest Price Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getBookTicker(const QString& symbol) {
    QUrl url("https://fapi.binance.com/fapi/v1/ticker/bookTicker");
    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleBookTickerResponse(reply); });
}

void binanceapi::handleBookTickerResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Book Ticker Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getOpenInterest(const QString& symbol) {
    if (symbol.isEmpty()) {
        qDebug() << "Symbol is mandatory for open interest request";
        return;
    }

    QUrl url("https://fapi.binance.com/fapi/v1/openInterest");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleOpenInterestResponse(reply); });
}

void binanceapi::handleOpenInterestResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Open Interest Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getOpenInterestHist(const QString& symbol, const QString& period, qint64 limit, qint64 startTime, qint64 endTime) {
    if (symbol.isEmpty()) {
        qDebug() << "Symbol is mandatory for open interest history request";
        return;
    }

    if (period.isEmpty()) {
        qDebug() << "Period is mandatory for open interest history request";
        return;
    }

    QUrl url("https://fapi.binance.com/futures/data/openInterestHist");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("period", period);
    if (limit > 0 && limit <= 500) {
        query.addQueryItem("limit", QString::number(limit));
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleOpenInterestHistResponse(reply); });
}

void binanceapi::handleOpenInterestHistResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Open Interest History Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getTopLongShortAccountRatio(const QString& symbol, const QString& period, qint64 limit, qint64 startTime, qint64 endTime) {
    if (symbol.isEmpty()) {
        qDebug() << "Symbol is mandatory for top long short account ratio request";
        return;
    }

    if (period.isEmpty()) {
        qDebug() << "Period is mandatory for top long short account ratio request";
        return;
    }

    QUrl url("https://fapi.binance.com/futures/data/topLongShortAccountRatio");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("period", period);
    if (limit > 0 && limit <= 500) {
        query.addQueryItem("limit", QString::number(limit));
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleTopLongShortAccountRatioResponse(reply); });
}

void binanceapi::handleTopLongShortAccountRatioResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Top Long Short Account Ratio Info:" << jsonObject;
    }
    reply->deleteLater();
}

void binanceapi::getTopLongShortPositionRatio(const QString& symbol, const QString& period, qint64 limit, qint64 startTime, qint64 endTime) {
    if (symbol.isEmpty()) {
        qDebug() << "Symbol is mandatory for top long short position ratio request";
        return;
    }

    if (period.isEmpty()) {
        qDebug() << "Period is mandatory for top long short position ratio request";
        return;
    }

    QUrl url("https://fapi.binance.com/futures/data/topLongShortPositionRatio");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("period", period);
    if (limit > 0 && limit <= 500) {
        query.addQueryItem("limit", QString::number(limit));
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleTopLongShortPositionRatioResponse(reply); });
}

void binanceapi::handleTopLongShortPositionRatioResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Top Long Short Position Ratio Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getGlobalLongShortAccountRatio(const QString& symbol, const QString& period, qint64 limit, qint64 startTime, qint64 endTime) {
    if (symbol.isEmpty()) {
        qDebug() << "Symbol is mandatory for global long short account ratio request";
        return;
    }

    if (period.isEmpty()) {
        qDebug() << "Period is mandatory for global long short account ratio request";
        return;
    }

    QUrl url("https://fapi.binance.com/futures/data/globalLongShortAccountRatio");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("period", period);
    if (limit > 0 && limit <= 500) {
        query.addQueryItem("limit", QString::number(limit));
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleGlobalLongShortAccountRatioResponse(reply); });
}

void binanceapi::handleGlobalLongShortAccountRatioResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Global Long Short Account Ratio Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getTakerLongShortRatio(const QString& symbol, const QString& period, qint64 limit, qint64 startTime, qint64 endTime) {
    if (symbol.isEmpty()) {
        qDebug() << "Symbol is mandatory for taker long short ratio request";
        return;
    }

    if (period.isEmpty()) {
        qDebug() << "Period is mandatory for taker long short ratio request";
        return;
    }

    QUrl url("https://fapi.binance.com/futures/data/takerlongshortRatio");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("period", period);
    if (limit > 0 && limit <= 500) {
        query.addQueryItem("limit", QString::number(limit));
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleTakerLongShortRatioResponse(reply); });
}

void binanceapi::handleTakerLongShortRatioResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Taker Long Short Ratio Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getLvtKlines(const QString& symbol, const QString& interval, qint64 startTime, qint64 endTime, int limit) {
    if (symbol.isEmpty()) {
        qDebug() << "Symbol is mandatory for LVT klines request";
        return;
    }

    if (interval.isEmpty()) {
        qDebug() << "Interval is mandatory for LVT klines request";
        return;
    }

    QUrl url("https://fapi.binance.com/fapi/v1/lvtKlines");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("interval", interval);
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    if (limit > 0 && limit <= 1000) {
        query.addQueryItem("limit", QString::number(limit));
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleLvtKlinesResponse(reply); });
}

void binanceapi::handleLvtKlinesResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "LVT Klines Info:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getIndexInfo(const QString& symbol) {
    QUrl url("https://fapi.binance.com/fapi/v1/indexInfo");
    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleIndexInfoResponse(reply); });
}

void binanceapi::handleIndexInfoResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Index Info:" << jsonObject;
    }
    reply->deleteLater();
}

void binanceapi::getAssetIndex(const QString& symbol) {
    QUrl url("https://fapi.binance.com/fapi/v1/assetIndex");
    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleAssetIndexResponse(reply); });
}

void binanceapi::handleAssetIndexResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Asset Index Info:" << jsonObject;
    }
    reply->deleteLater();
}


void binanceapi::changePositionMode(bool dualSidePosition, qint64 recvWindow, qint64 timestamp) {
    QString dualSidePositionStr = dualSidePosition ? "true" : "false";
    QString timestampStr = QString::number(timestamp);
    QByteArray payload = QString("dualSidePosition=%1&recvWindow=%2&timestamp=%3").arg(dualSidePositionStr, QString::number(recvWindow), timestampStr).toUtf8();

    QUrl url("https://fapi.binance.com/fapi/v1/positionSide/dual");
    QNetworkRequest request(url);

    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QByteArray signature = QMessageAuthenticationCode::hash(payload, apiSecret.toUtf8(), QCryptographicHash::Sha256).toHex();
    request.setRawHeader("signature", signature);

    QNetworkReply* reply = networkManager.post(request, payload);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleChangePositionModeResponse(reply); });
}

void binanceapi::handleChangePositionModeResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Change Position Mode Response:" << jsonObject;
    }
    reply->deleteLater();
}

void binanceapi::getPositionMode(qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/positionSide/dual");
    QUrlQuery query;
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QByteArray signature = QMessageAuthenticationCode::hash(url.toEncoded(), apiSecret.toUtf8(), QCryptographicHash::Sha256).toHex();
    request.setRawHeader("signature", signature);

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handlePositionModeResponse(reply); });
}

void binanceapi::handlePositionModeResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Position Mode Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::changeMultiAssetsMode(bool multiAssetsMargin, qint64 recvWindow, qint64 timestamp) {
    QString multiAssetsMarginStr = multiAssetsMargin ? "true" : "false";
    QString timestampStr = QString::number(timestamp);
    QByteArray payload = QString("multiAssetsMargin=%1&recvWindow=%2&timestamp=%3").arg(multiAssetsMarginStr, QString::number(recvWindow), timestampStr).toUtf8();

    QUrl url("https://fapi.binance.com/fapi/v1/multiAssetsMargin");
    QNetworkRequest request(url);

    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QByteArray signature = QMessageAuthenticationCode::hash(payload, apiSecret.toUtf8(), QCryptographicHash::Sha256).toHex();
    request.setRawHeader("signature", signature);

    QNetworkReply* reply = networkManager.post(request, payload);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleChangeMultiAssetsModeResponse(reply); });
}

void binanceapi::handleChangeMultiAssetsModeResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Change Multi-Assets Mode Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::sendNewOrder(const QString& symbol, const QString& side, const QString& positionSide, const QString& type,
                              const QString& timeInForce, const QString& quantity, const QString& reduceOnly,
                              const QString& price, const QString& newClientOrderId, const QString& stopPrice,
                              const QString& closePosition, const QString& activationPrice, const QString& callbackRate,
                              const QString& workingType, const QString& priceProtect, const QString& newOrderRespType,
                              qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/order");
    QNetworkRequest request(url);

    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("side", side);
    if (!positionSide.isEmpty()) {
        query.addQueryItem("positionSide", positionSide);
    }
    query.addQueryItem("type", type);
    if (!timeInForce.isEmpty()) {
        query.addQueryItem("timeInForce", timeInForce);
    }
    if (!quantity.isEmpty()) {
        query.addQueryItem("quantity", quantity);
    }
    if (!reduceOnly.isEmpty()) {
        query.addQueryItem("reduceOnly", reduceOnly);
    }
    if (!price.isEmpty()) {
        query.addQueryItem("price", price);
    }
    if (!newClientOrderId.isEmpty()) {
        query.addQueryItem("newClientOrderId", newClientOrderId);
    }
    if (!stopPrice.isEmpty()) {
        query.addQueryItem("stopPrice", stopPrice);
    }
    if (!closePosition.isEmpty()) {
        query.addQueryItem("closePosition", closePosition);
    }
    if (!activationPrice.isEmpty()) {
        query.addQueryItem("activationPrice", activationPrice);
    }
    if (!callbackRate.isEmpty()) {
        query.addQueryItem("callbackRate", callbackRate);
    }
    if (!workingType.isEmpty()) {
        query.addQueryItem("workingType", workingType);
    }
    if (!priceProtect.isEmpty()) {
        query.addQueryItem("priceProtect", priceProtect);
    }
    if (!newOrderRespType.isEmpty()) {
        query.addQueryItem("newOrderRespType", newOrderRespType);
    }
    url.setQuery(query);

    QByteArray payload = url.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority);

    request.setRawHeader("signature", QMessageAuthenticationCode::hash(payload, apiSecret.toUtf8() , QCryptographicHash::Sha256).toHex());

    QNetworkReply* reply = networkManager.post(request, payload);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleNewOrderResponse(reply); });
}

void binanceapi::handleNewOrderResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "New Order Response:" << jsonObject;
    }
    reply->deleteLater();
}


void binanceapi::modifyOrder(qint64 orderId, const QString& origClientOrderId, const QString& symbol, const QString& side,
                             const QString& quantity, const QString& price, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/order");
    QUrlQuery query;

    if (orderId >= 0) {
        query.addQueryItem("orderId", QString::number(orderId));
    }
    if (!origClientOrderId.isEmpty()) {
        query.addQueryItem("origClientOrderId", origClientOrderId);
    }
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("side", side);
    query.addQueryItem("quantity", quantity);
    query.addQueryItem("price", price);
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QByteArray payload = url.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("signature", QMessageAuthenticationCode::hash(payload, apiSecret.toUtf8(), QCryptographicHash::Sha256).toHex());

    QNetworkReply* reply = networkManager.put(request, payload);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleModifyOrderResponse(reply); });
}

void binanceapi::handleModifyOrderResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Modify Order Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::batchOrders(const QJsonArray& orderList, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/batchOrders");
    QUrlQuery query;

    query.addQueryItem("batchOrders", QJsonDocument(orderList).toJson());
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QByteArray payload = url.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("signature", QMessageAuthenticationCode::hash(payload, apiSecret.toUtf8(), QCryptographicHash::Sha256).toHex());

    QNetworkReply* reply = networkManager.post(request, payload);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleBatchOrdersResponse(reply); });
}

void binanceapi::handleBatchOrdersResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Batch Orders Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::batchModifyOrders(const QJsonArray& orderList, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/batchOrders");
    QUrlQuery query;

    query.addQueryItem("batchOrders", QJsonDocument(orderList).toJson());
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QByteArray payload = url.toEncoded(QUrl::RemoveScheme | QUrl::RemoveAuthority);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("signature", QMessageAuthenticationCode::hash(payload, apiSecret.toUtf8(), QCryptographicHash::Sha256).toHex());

    QNetworkReply* reply = networkManager.put(request, payload);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleBatchModifyOrdersResponse(reply); });
}

void binanceapi::handleBatchModifyOrdersResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Batch Modify Orders Response:" << jsonObject;
    }
    reply->deleteLater();
}

void binanceapi::getOrderAmendmentHistory(const QString& symbol, qint64 orderId, const QString& origClientOrderId, qint64 startTime, qint64 endTime, int limit, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/orderAmendment");
    QUrlQuery query;

    query.addQueryItem("symbol", symbol);
    if (orderId >= 0) {
        query.addQueryItem("orderId", QString::number(orderId));
    }
    if (!origClientOrderId.isEmpty()) {
        query.addQueryItem("origClientOrderId", origClientOrderId);
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    if (limit > 0) {
        query.addQueryItem("limit", QString::number(limit));
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleOrderAmendmentHistoryResponse(reply); });
}

void binanceapi::handleOrderAmendmentHistoryResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Order Amendment History Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::cancelOrder(const QString& symbol, qint64 orderId, const QString& origClientOrderId, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/order");
    QUrlQuery query;

    query.addQueryItem("symbol", symbol);
    if (orderId >= 0) {
        query.addQueryItem("orderId", QString::number(orderId));
    }
    if (!origClientOrderId.isEmpty()) {
        query.addQueryItem("origClientOrderId", origClientOrderId);
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.deleteResource(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleCancelOrderResponse(reply); });
}

void binanceapi::handleCancelOrderResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Cancel Order Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::cancelAllOpenOrders(const QString& symbol, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/allOpenOrders");
    QUrlQuery query;

    query.addQueryItem("symbol", symbol);
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.deleteResource(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleCancelAllOpenOrdersResponse(reply); });
}

void binanceapi::handleCancelAllOpenOrdersResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Cancel All Open Orders Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::cancelBatchOrders(const QString& symbol, const QList<qint64>& orderIdList, const QList<QString>& origClientOrderIdList, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/batchOrders");
    QUrlQuery query;

    query.addQueryItem("symbol", symbol);

    if (!orderIdList.isEmpty()) {
        QStringList orderIdListString;
        for (const qint64 orderId : orderIdList) {
            orderIdListString << QString::number(orderId);
        }
        query.addQueryItem("orderIdList", "[" + orderIdListString.join(",") + "]");
    }

    if (!origClientOrderIdList.isEmpty()) {
        QStringList origClientOrderIdListEncoded;
        for (const QString& origClientOrderId : origClientOrderIdList) {
            origClientOrderIdListEncoded << "\"" + origClientOrderId + "\"";
        }
        query.addQueryItem("origClientOrderIdList", "[" + origClientOrderIdListEncoded.join(",") + "]");
    }

    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.deleteResource(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleCancelBatchOrdersResponse(reply); });
}

void binanceapi::handleCancelBatchOrdersResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Cancel Batch Orders Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::countdownCancelAll(const QString& symbol, qint64 countdownTime, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/countdownCancelAll");
    QUrlQuery query;

    query.addQueryItem("symbol", symbol);
    query.addQueryItem("countdownTime", QString::number(countdownTime));

    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.post(request, query.toString().toUtf8());
    connect(reply, &QNetworkReply::finished, this, [=]() { handleCountdownCancelAllResponse(reply); });
}

void binanceapi::handleCountdownCancelAllResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Countdown Cancel All Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getOpenOrder(const QString& symbol, qint64 orderId, const QString& origClientOrderId, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/openOrder");
    QUrlQuery query;

    query.addQueryItem("symbol", symbol);

    if (orderId >= 0) {
        query.addQueryItem("orderId", QString::number(orderId));
    }
    if (!origClientOrderId.isEmpty()) {
        query.addQueryItem("origClientOrderId", origClientOrderId);
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleOpenOrderResponse(reply); });
}

void binanceapi::handleOpenOrderResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Open Order Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getOpenOrders(const QString& symbol, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/openOrders");
    QUrlQuery query;

    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleOpenOrdersResponse(reply); });
}

void binanceapi::handleOpenOrdersResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Open Orders Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getAllOrders(const QString& symbol, qint64 orderId, qint64 startTime, qint64 endTime, int limit, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/allOrders");
    QUrlQuery query;

    query.addQueryItem("symbol", symbol);

    if (orderId >= 0) {
        query.addQueryItem("orderId", QString::number(orderId));
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    if (limit >= 0) {
        query.addQueryItem("limit", QString::number(limit));
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleAllOrdersResponse(reply); });
}

void binanceapi::handleAllOrdersResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "All Orders Response:" << jsonObject;
    }
    reply->deleteLater();
}

void binanceapi::getBalance(qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v2/balance");
    QUrlQuery query;

    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleBalanceResponse(reply); });
}

void binanceapi::handleBalanceResponse(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Balance Response:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getAccountInformation(qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v2/account");
    QUrlQuery query;

    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleAccountInformation(reply); });
}

void binanceapi::handleAccountInformation(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Account Information:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::changeLeverage(const QString& symbol, int leverage, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/leverage");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("leverage", QString::number(leverage));

    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.post(request, QByteArray());
    connect(reply, &QNetworkReply::finished, this, [=]() { handleLeverageChange(reply); });
}

void binanceapi::handleLeverageChange(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Leverage Change Result:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::changeMarginType(const QString& symbol, const QString& marginType, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/marginType");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("marginType", marginType);

    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.post(request, QByteArray());
    connect(reply, &QNetworkReply::finished, this, [=]() { handleMarginTypeChange(reply); });
}

void binanceapi::handleMarginTypeChange(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Margin Type Change Result:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::adjustPositionMargin(const QString& symbol, const QString& positionSide, double amount, int type, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/positionMargin");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    if (!positionSide.isEmpty()) {
        query.addQueryItem("positionSide", positionSide);
    }
    query.addQueryItem("amount", QString::number(amount));
    query.addQueryItem("type", QString::number(type));

    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.post(request, QByteArray());
    connect(reply, &QNetworkReply::finished, this, [=]() { handlePositionMarginAdjustment(reply); });
}

void binanceapi::handlePositionMarginAdjustment(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Position Margin Adjustment Result:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getPositionMarginHistory(const QString& symbol, int type, qint64 startTime, qint64 endTime, int limit, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v1/positionMargin/history");
    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    if (type > 0) {
        query.addQueryItem("type", QString::number(type));
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    if (limit > 0) {
        query.addQueryItem("limit", QString::number(limit));
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handlePositionMarginHistory(reply); });
}

void binanceapi::handlePositionMarginHistory(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "Position Margin History Result:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getPositionRisk(const QString& symbol, qint64 recvWindow, qint64 timestamp) {
    QUrl url("https://fapi.binance.com/fapi/v2/positionRisk");
    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handlePositionRisk(reply); });
}

void binanceapi::handlePositionRisk(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonValue dataValue = jsonObject.value("data");
        if (dataValue.isArray()) {
            QJsonArray jsonArray = dataValue.toArray();
            qDebug() << "Position Risk Result:" << jsonArray;
        } else {
            qDebug() << "Invalid data format received.";
        }
    }
    reply->deleteLater();
}
void binanceapi::getUserTrades(const QString& symbol, qint64 orderId, qint64 startTime, qint64 endTime, qint64 fromId, int limit, qint64 recvWindow, qint64 timestamp)
{
    QUrl url("https://fapi.binance.com/fapi/v1/userTrades");

    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    if (orderId >= 0) {
        query.addQueryItem("orderId", QString::number(orderId));
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    if (fromId >= 0) {
        query.addQueryItem("fromId", QString::number(fromId));
    }
    if (limit > 0) {
        query.addQueryItem("limit", QString::number(limit));
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleUserTrades(reply); });
}

void binanceapi::handleUserTrades(QNetworkReply* reply)
{
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonArray tradesArray = jsonResponse.array();
        qDebug() << "User Trades Result:" << tradesArray;
    }
    reply->deleteLater();
}
void binanceapi::getIncome(const QString& symbol, const QString& incomeType, qint64 startTime, qint64 endTime, int limit, qint64 recvWindow, qint64 timestamp)
{
    QUrl url("https://fapi.binance.com/fapi/v1/income");

    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    if (!incomeType.isEmpty()) {
        query.addQueryItem("incomeType", incomeType);
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    if (limit > 0) {
        query.addQueryItem("limit", QString::number(limit));
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleIncome(reply); });
}

void binanceapi::handleIncome(QNetworkReply* reply)
{
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonArray incomeArray = jsonResponse.array();
        qDebug() << "Income Result:" << incomeArray;
    }
    reply->deleteLater();
}
void binanceapi::getLeverageBracket(const QString& symbol, qint64 recvWindow, qint64 timestamp)
{
    QUrl url("https://fapi.binance.com/fapi/v1/leverageBracket");

    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleLeverageBracket(reply); });
}

void binanceapi::handleLeverageBracket(QNetworkReply* reply)
{
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonArray leverageBracketArray = jsonResponse.array();
        qDebug() << "Leverage Bracket Result:" << leverageBracketArray;
    }
    reply->deleteLater();
}
void binanceapi::getAdlQuantile(const QString& symbol, qint64 recvWindow, qint64 timestamp)
{
    QUrl url("https://fapi.binance.com/fapi/v1/adlQuantile");

    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleAdlQuantile(reply); });
}

void binanceapi::handleAdlQuantile(QNetworkReply* reply)
{
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        qDebug() << "ADL Quantile Result:" << jsonObject;
    }
    reply->deleteLater();
}
void binanceapi::getForceOrders(const QString& symbol, const QString& autoCloseType, qint64 startTime, qint64 endTime, int limit, qint64 recvWindow, qint64 timestamp)
{
    QUrl url("https://fapi.binance.com/fapi/v1/forceOrders");

    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    if (!autoCloseType.isEmpty()) {
        query.addQueryItem("autoCloseType", autoCloseType);
    }
    if (startTime >= 0) {
        query.addQueryItem("startTime", QString::number(startTime));
    }
    if (endTime >= 0) {
        query.addQueryItem("endTime", QString::number(endTime));
    }
    if (limit > 0) {
        query.addQueryItem("limit", QString::number(limit));
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleForceOrders(reply); });
}

void binanceapi::handleForceOrders(QNetworkReply* reply)
{
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonValue dataValue = jsonObject.value("data");
        if (dataValue.isArray()) {
            QJsonArray jsonArray = dataValue.toArray();
            qDebug() << "Force Orders Result:" << jsonArray;
        } else {
            qDebug() << "Invalid data format received.";
        }
    }
    reply->deleteLater();
}
void binanceapi::getApiTradingStatus(const QString& symbol, qint64 recvWindow, qint64 timestamp)
{
    QUrl url("https://fapi.binance.com/fapi/v1/apiTradingStatus");

    QUrlQuery query;
    if (!symbol.isEmpty()) {
        query.addQueryItem("symbol", symbol);
    }
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    if (timestamp >= 0) {
        query.addQueryItem("timestamp", QString::number(timestamp));
    }

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleApiTradingStatus(reply); });
}

void binanceapi::handleApiTradingStatus(QNetworkReply* reply)
{
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonValue dataValue = jsonObject.value("data");
        if (dataValue.isArray()) {
            QJsonArray jsonArray = dataValue.toArray();
            qDebug() << "API Trading Status Result:" << jsonArray;
        } else {
            qDebug() << "Invalid data format received.";
        }
    }
    reply->deleteLater();
}
void binanceapi::getCommissionRate(const QString& symbol, qint64 recvWindow, qint64 timestamp)
{
    QUrl url("https://fapi.binance.com/fapi/v1/commissionRate");

    QUrlQuery query;
    query.addQueryItem("symbol", symbol);
    if (recvWindow >= 0) {
        query.addQueryItem("recvWindow", QString::number(recvWindow));
    }
    query.addQueryItem("timestamp", QString::number(timestamp));

    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("X-MBX-APIKEY", apiKey.toUtf8());

    QNetworkReply* reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() { handleCommissionRate(reply); });
}

void binanceapi::handleCommissionRate(QNetworkReply* reply)
{
    if (reply->error()) {
        qDebug() << "There was an error with the request:" << reply->errorString();
    } else {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonValue dataValue = jsonObject.value("data");
        if (dataValue.isObject()) {
            QJsonObject dataObject = dataValue.toObject();
            qDebug() << "Commission Rate Result:" << dataObject;
        } else {
            qDebug() << "Invalid data format received.";
        }
    }
    reply->deleteLater();
}


void binanceapi::createUserDataStream()
{
    QNetworkRequest request(QUrl("https://fapi.binance.com/fapi/v1/listenKey"));
    QNetworkReply *reply = networkManager.post(request, QByteArray());
    connect(reply, &QNetworkReply::finished, this, &binanceapi::onCreateUserDataStreamFinished);
}

void binanceapi::extendUserDataStream(const QString &listenKey)
{
    QUrl url("https://fapi.binance.com/fapi/v1/listenKey");
    QUrlQuery query;
    query.addQueryItem("listenKey", listenKey);
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager.put(request, QByteArray());
    connect(reply, &QNetworkReply::finished, this, &binanceapi::onExtendUserDataStreamFinished);
}

void binanceapi::closeUserDataStream(const QString &listenKey)
{
    QUrl url("https://fapi.binance.com/fapi/v1/listenKey");
    QUrlQuery query;
    query.addQueryItem("listenKey", listenKey);
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager.deleteResource(request);
    connect(reply, &QNetworkReply::finished, this, &binanceapi::onCloseUserDataStreamFinished);
}

void binanceapi::onCreateUserDataStreamFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QString listenKey = jsonDoc.object().value("listenKey").toString();
        qDebug() << "User Data Stream created. Listen Key:" << listenKey;
    }
    else
    {
        qDebug() << "Error creating User Data Stream:" << reply->errorString();
    }
    reply->deleteLater();
}

void binanceapi::onExtendUserDataStreamFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error() == QNetworkReply::NoError)
    {
        qDebug() << "User Data Stream extended successfully.";
    }
    else
    {
        qDebug() << "Error extending User Data Stream:" << reply->errorString();
    }
    reply->deleteLater();
}

void binanceapi::onCloseUserDataStreamFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error() == QNetworkReply::NoError)
    {
        qDebug() << "User Data Stream closed successfully.";
    }
    else
    {
        qDebug() << "Error closing User Data Stream:" << reply->errorString();
    }
    reply->deleteLater();
}





