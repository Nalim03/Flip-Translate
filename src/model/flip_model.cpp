#include "flip_model.h"

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

const QMap<FlipModel::Language, QString> FlipModel::languageToCode = {
    {Auto, QStringLiteral("auto")},
    {Afrikaans, QStringLiteral("af")},
    {Albanian, QStringLiteral("sq")},
    {Amharic, QStringLiteral("am")},
    {Arabic, QStringLiteral("ar")},
    {Armenian, QStringLiteral("hy")},
    {Azerbaijani, QStringLiteral("az")},
    {Bashkir, QStringLiteral("ba")},
    {Basque, QStringLiteral("eu")},
    {Belarusian, QStringLiteral("be")},
    {Bengali, QStringLiteral("bn")},
    {Bosnian, QStringLiteral("bs")},
    {Bulgarian, QStringLiteral("bg")},
    {Cantonese, QStringLiteral("yue")},
    {Catalan, QStringLiteral("ca")},
    {Cebuano, QStringLiteral("ceb")},
    {Chichewa, QStringLiteral("ny")},
    {Corsican, QStringLiteral("co")},
    {Croatian, QStringLiteral("hr")},
    {Czech, QStringLiteral("cs")},
    {Danish, QStringLiteral("da")},
    {Dutch, QStringLiteral("nl")},
    {English, QStringLiteral("en")},
    {Esperanto, QStringLiteral("eo")},
    {Estonian, QStringLiteral("et")},
    {Fijian, QStringLiteral("fj")},
    {Filipino, QStringLiteral("fil")},
    {Finnish, QStringLiteral("fi")},
    {French, QStringLiteral("fr")},
    {Frisian, QStringLiteral("fy")},
    {Galician, QStringLiteral("gl")},
    {Georgian, QStringLiteral("ka")},
    {German, QStringLiteral("de")},
    {Greek, QStringLiteral("el")},
    {Gujarati, QStringLiteral("gu")},
    {HaitianCreole, QStringLiteral("ht")},
    {Hausa, QStringLiteral("ha")},
    {Hawaiian, QStringLiteral("haw")},
    {Hebrew, QStringLiteral("iw")},
    {HillMari, QStringLiteral("mrj")},
    {Hindi, QStringLiteral("hi")},
    {Hmong, QStringLiteral("hmn")},
    {Hungarian, QStringLiteral("hu")},
    {Icelandic, QStringLiteral("is")},
    {Igbo, QStringLiteral("ig")},
    {Indonesian, QStringLiteral("id")},
    {Irish, QStringLiteral("ga")},
    {Italian, QStringLiteral("it")},
    {Japanese, QStringLiteral("ja")},
    {Javanese, QStringLiteral("jw")},
    {Kannada, QStringLiteral("kn")},
    {Kazakh, QStringLiteral("kk")},
    {Khmer, QStringLiteral("km")},
    {Kinyarwanda, QStringLiteral("rw")},
    {Klingon, QStringLiteral("tlh")},
    {KlingonPlqaD, QStringLiteral("tlh-Qaak")},
    {Korean, QStringLiteral("ko")},
    {Kurdish, QStringLiteral("ku")},
    {Kyrgyz, QStringLiteral("ky")},
    {Lao, QStringLiteral("lo")},
    {Latin, QStringLiteral("la")},
    {Latvian, QStringLiteral("lv")},
    {LevantineArabic, QStringLiteral("apc")},
    {Lithuanian, QStringLiteral("lt")},
    {Luxembourgish, QStringLiteral("lb")},
    {Macedonian, QStringLiteral("mk")},
    {Malagasy, QStringLiteral("mg")},
    {Malay, QStringLiteral("ms")},
    {Malayalam, QStringLiteral("ml")},
    {Maltese, QStringLiteral("mt")},
    {Maori, QStringLiteral("mi")},
    {Marathi, QStringLiteral("mr")},
    {Mari, QStringLiteral("mhr")},
    {Mongolian, QStringLiteral("mn")},
    {Myanmar, QStringLiteral("my")},
    {Nepali, QStringLiteral("ne")},
    {Norwegian, QStringLiteral("no")},
    {Oriya, QStringLiteral("or")},
    {Papiamento, QStringLiteral("pap")},
    {Pashto, QStringLiteral("ps")},
    {Persian, QStringLiteral("fa")},
    {Polish, QStringLiteral("pl")},
    {Portuguese, QStringLiteral("pt")},
    {Punjabi, QStringLiteral("pa")},
    {QueretaroOtomi, QStringLiteral("otq")},
    {Romanian, QStringLiteral("ro")},
    {Russian, QStringLiteral("ru")},
    {Samoan, QStringLiteral("sm")},
    {ScotsGaelic, QStringLiteral("gd")},
    {SerbianCyrillic, QStringLiteral("sr")},
    {SerbianLatin, QStringLiteral("sr-Latin")},
    {Sesotho, QStringLiteral("st")},
    {Shona, QStringLiteral("sn")},
    {SimplifiedChinese, QStringLiteral("zh-CN")},
    {Sindhi, QStringLiteral("sd")},
    {Sinhala, QStringLiteral("si")},
    {Slovak, QStringLiteral("sk")},
    {Slovenian, QStringLiteral("sl")},
    {Somali, QStringLiteral("so")},
    {Spanish, QStringLiteral("es")},
    {Sundanese, QStringLiteral("su")},
    {Swahili, QStringLiteral("sw")},
    {Swedish, QStringLiteral("sv")},
    {Tagalog, QStringLiteral("tl")},
    {Tahitian, QStringLiteral("ty")},
    {Tajik, QStringLiteral("tg")},
    {Tamil, QStringLiteral("ta")},
    {Tatar, QStringLiteral("tt")},
    {Telugu, QStringLiteral("te")},
    {Thai, QStringLiteral("th")},
    {Tongan, QStringLiteral("to")},
    {TraditionalChinese, QStringLiteral("zh-TW")},
    {Turkish, QStringLiteral("tr")},
    {Turkmen, QStringLiteral("tk")},
    {Udmurt, QStringLiteral("udm")},
    {Uighur, QStringLiteral("ug")},
    {Ukrainian, QStringLiteral("uk")},
    {Urdu, QStringLiteral("ur")},
    {Uzbek, QStringLiteral("uz")},
    {Vietnamese, QStringLiteral("vi")},
    {Welsh, QStringLiteral("cy")},
    {Xhosa, QStringLiteral("xh")},
    {Yiddish, QStringLiteral("yi")},
    {Yoruba, QStringLiteral("yo")},
    {YucatecMaya, QStringLiteral("yua")},
    {Zulu, QStringLiteral("zu")}
};

void FlipModel::translate(Language from, Language to, QString sentece) {
    QString urlString = APIUrl.arg(languageToCode[from], languageToCode[to], sentece);
    const QUrl url = QUrl(urlString);

    QNetworkAccessManager *man = new QNetworkAccessManager;
    QNetworkRequest request(url);

    QNetworkReply* reply =  man->get(request);

    connect(reply, &QNetworkReply::readyRead, this, &FlipModel::translateFinished);
};

void FlipModel::translateFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QString replyString = reply->readAll();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(replyString.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();
    QJsonArray jsonArray = jsonObj["sentences"].toArray();
    QString translatedWord = jsonArray[0].toObject()["trans"].toString();

    translated(translatedWord);
    qInfo() << translatedWord;
};
