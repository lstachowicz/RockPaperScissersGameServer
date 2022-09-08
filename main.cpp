//
//  Created by Łukasz Stachowicz on 01/09/2022.
//

#include "game.hpp"

#include <QCoreApplication>
#include <QtHttpServer/QtHttpServer>

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);

    QHttpServer server;

    std::shared_ptr<RPSGame::GameInterface> game(RPSGame::createGame());

    server.route("/", QHttpServerRequest::Method::Get, [&] (QHttpServerResponder &&responder) {
        QFile file("../RockPaperScissersGameClient/client.html");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        responder.write(file.readAll(), {{"Access-Control-Allow-Origin", "*"}, }, QHttpServerResponder::StatusCode::Ok);
    });

    server.route("/assets/<arg>", [] (const QUrl &url) {
        return QHttpServerResponse::fromFile(QStringLiteral("../RockPaperScissersGameClient/assets/%1").arg(url.path()));
    });

    server.route("/join", QHttpServerRequest::Method::Get, [&] (QHttpServerResponder &&responder) {
        std::string player_hash;
        if (game->createPlayer(player_hash))
        {
            QJsonObject json { {"playerId", player_hash.c_str() } };
            QJsonDocument doc(json);
            responder.write(doc, {{"Access-Control-Allow-Origin", "*"}}, QHttpServerResponder::StatusCode::Ok);
        }
        else
        {
            responder.write("", {{"Access-Control-Allow-Origin", "*"}}, QHttpServerResponder::StatusCode::InternalServerError);
        }
    });

    // http://localhost:8080/join/DC77C5AC-E4FD-4393-BF81-3A7916D13061
    server.route("/join/<arg>", QHttpServerRequest::Method::Get, [&] (const QString& player_id, QHttpServerResponder &&responder) {
        std::string player_hash { player_id.isEmpty() ? "" : player_id.toUtf8().toStdString()};
        if (game->createPlayer(player_hash))
        {
            QJsonObject json { {"playerId", player_hash.c_str() } };
            QJsonDocument doc(json);
            responder.write(doc, {{"Access-Control-Allow-Origin", "*"}}, QHttpServerResponder::StatusCode::Ok);
        }
        else
        {
            responder.write("", {{"Access-Control-Allow-Origin", "*"}}, QHttpServerResponder::StatusCode::InternalServerError);
        }
    });

    // http://localhost:8080/move/DC77C5AC-E4FD-4393-BF81-3A7916D13061
    server.route("/move/<arg>", QHttpServerRequest::Method::Post, [&] (const QString& player_id, QHttpServerResponder &&responder, const QHttpServerRequest& request) {
        std::string action;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(request.body(), &error);
        if (doc.isObject())
        {
            const QJsonObject json = doc.object();
            action = json["action"].toString().toUtf8().data();
        }

        if (action.empty())
        {
            responder.write("", {{"Access-Control-Allow-Origin", "*"}}, QHttpServerResponder::StatusCode::NoContent);
        }

        if (game->move(player_id.toUtf8().toStdString(), action))
        {
            responder.write("", {{"Access-Control-Allow-Origin", "*"}}, QHttpServerResponder::StatusCode::Ok);
        }
        else
        {
            responder.write("", {{"Access-Control-Allow-Origin", "*"}}, QHttpServerResponder::StatusCode::Forbidden);
        }
    });

    // http://localhost:8080/events/DC77C5AC-E4FD-4393-BF81-3A7916D13061
    server.route("/events/<arg>", QHttpServerRequest::Method::Get, [&] (const QString& player_id, QHttpServerResponder &&responder) {
        std::list<std::string> events;
        if (game->events(player_id.toUtf8().data(), events))
        {

            QJsonArray array { };
            for (auto it = events.begin(); it != events.end(); ++it)
            {
                array.append(it->c_str());
            }
            QJsonObject json { {"events", array} };
            QJsonDocument doc(json);
            responder.write(doc, {{"Access-Control-Allow-Origin", "*"}}, QHttpServerResponder::StatusCode::Ok);
        }
        else
        {
            responder.write("", {{"Access-Control-Allow-Origin", "*"}}, QHttpServerResponder::StatusCode::NotFound);
        }
    });

    server.route("/rank", QHttpServerRequest::Method::Get, [&] (QHttpServerResponder &&responder) {
        std::multimap<uint64_t, std::string> rank;
        game->rank(rank);

        QJsonArray array { };
        for (auto it = rank.begin(); it != rank.end(); ++it)
        {
            QJsonObject object { {"score", QString::number(it->first)}, {"player", it->second.c_str()} };
            array.append(object);
        }

        QJsonObject json { {"rank", array} };
        QJsonDocument doc(json);
        responder.write(doc, {{"Access-Control-Allow-Origin", "*"}}, QHttpServerResponder::StatusCode::Ok);
    });

    server.listen(QHostAddress::Any, 8080);

    return application.exec();
}
