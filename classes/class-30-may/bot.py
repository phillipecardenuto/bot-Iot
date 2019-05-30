import logging
import telegram
from telegram.ext import CommandHandler
from telegram.ext import Updater
import pandas as pd
import re
import datetime
import paho.mqtt.client as mqtt
import json
# Setting up logger
logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger()
logger.setLevel(logging.INFO)
#token_telegram = '666164429:AAFPkMnbqRKXqCPvv-OtycJ5K5y4RQ3k_U4'
token_vina = '850097657:AAHEHdl6HrpDYKb2_9nKPFDx1eQkBMyW7xg'
bot = telegram.Bot(token=token_vina)

##### CLIENT MQTT

def on_connect(client, userdata, flags, rc):
    print("Connected!")
    client.subscribe("data/ou83rio3h43j/sub/in")

user = "ou83rio3h43j"
password = "anioaDro9R87"
publishAddress = "data/ou83rio3h43j/pub/out"
subscribeAddress = "data/ou83rio3h43j/sub/in"
client = mqtt.Client()
client.on_connect = on_connect
client.username_pw_set(user, password)



# Funções de Crawler do Cardápio
global guarnicao
global prato_principal
global salada
global sobremesa
global suco
global obs
guarnicao= "GUARNIÇÃO"
prato_principal= "PRATO PRINCIPAL:"
salada= "SALADA:"
sobremesa= "SOBREMESA:"
suco= "SUCO:"
obs= "OBSERVAÇÕES:"
def extrai_refeicao(ref,ref_op):
    if ref_op == guarnicao and prato_principal in ref :
        return ref[0:ref.find(prato_principal)].strip()
    if ref_op == prato_principal and prato_principal in ref and salada in ref:
        pp = ref.find(prato_principal) + len(prato_principal)
        return ref[pp:ref.find(salada)].strip()
    elif ref_op == salada and salada in ref and sobremesa in ref:
        sp = ref.find(salada) + len(salada)
        return ref[sp:ref.find(sobremesa)].strip()
    elif ref_op == sobremesa and sobremesa in ref and suco in ref:
        sp = ref.find(sobremesa) + len(sobremesa)
        return ref[sp:ref.find(suco)].strip()
    elif ref_op == suco and suco in ref and obs in ref:
        sp = ref.find(suco) + len(suco)
        return ref[sp:ref.find(obs)].strip()
    elif ref_op == obs and obs in ref:
        op = ref.find(obs) + len(obs)
        return ref[op:].strip()
    else: return ""
def cardapio_em_dia(dia_cardapio,hoje):
    """
    retorna  True se estiver atualizado
    retorna False se estiver desatualizado
    """
    d_dia_cardapio = datetime.datetime(year=dia_cardapio[2],month=dia_cardapio[1],day=dia_cardapio[0])
    d_hoje = datetime.datetime(year=hoje[2],month=hoje[1],day=hoje[0])
    if d_dia_cardapio >= d_hoje:
        return True
    return False
def atualiza_cardapio():
    """
    Verifica e atualiza o cardápio
    """
    hoje = datetime.datetime.now()
    hoje = [hoje.day,hoje.month,hoje.year]
    if not "dia_cardapio" in globals():
        global dia_cardapio
        dia_cardapio = []
    if hoje != dia_cardapio:
        if dia_cardapio == []:
            atualiza_tabela_cardapio()
        elif not cardapio_em_dia(dia_cardapio,hoje):
            atualiza_tabela_cardapio()
def atualiza_tabela_cardapio():
    global almoco
    global almoco_veg
    global jantar
    global jantar_veg
    global cafe
    global dia_semana
    global dia_cardapio

    # Scrapy data
    tables = pd.read_html("https://www.prefeitura.unicamp.br/apps/site/cardapio.php")

    # Parsing das refeições
    cardapio = tables[1].loc[2:].copy()
    cardapio_header = cardapio.iloc[0]
    cardapio_header.name = ''
    cardapio = pd.DataFrame(cardapio.values[1:], columns=cardapio_header)
    cafe = tables[1].loc[0:1].copy()
    cafe = cafe.drop(cafe.columns[1:],axis=1)
    cafe_header =cafe.iloc[0]
    cafe = cafe.iloc[1]
    cafe_header.name = ''
    cafe = pd.DataFrame(cafe.values, columns=cafe_header)
    cardapio = pd.concat([cardapio, cafe],axis=1)
    data = tables[0][0][0]

    # REFEIÇÕES
    almoco = cardapio["Almoço"].values[0].upper()
    almoco_veg = cardapio["Almoço Vegetariano"].values[0].upper()
    jantar = cardapio["Jantar"].values[0].upper()
    jantar_veg = cardapio["Jantar Vegetariano"].values[0].upper()
    cafe = cardapio["Café da manhã"].values[0].upper()
    dia_cardapio = [int(data) for data in (re.findall('\d{2}/\d{2}/\d{4}',data)[0].split("/")) ]
    dia_semana = data[data.find("(")+1:data.find(")")]



# Funções de Handler do BOT
def start(bot,update):
    user_name = update.message.chat.first_name
    message= "Hello %s\n"%(user_name)
    bot.send_message(chat_id=update.message.chat_id, text=message)
    print(update)
def almoco_bot(bot,update):
    atualiza_cardapio()
    user_name = update.message.chat.first_name
    message= """
Olá %s,
Almoço de %s (%s) teremos:
-Guarnição: %s

-Prato Principal: %s

-Salada: %s

-Sobremesa: %s

-Suco: %s

Obs: %s"""%(
        user_name,
        "%d/%d/%d"%(dia_cardapio[0],dia_cardapio[1],dia_cardapio[2]),
        dia_semana,
        extrai_refeicao(almoco,guarnicao),
        extrai_refeicao(almoco,prato_principal),
        extrai_refeicao(almoco,salada),
        extrai_refeicao(almoco,sobremesa),
        extrai_refeicao(almoco,suco),
        extrai_refeicao(almoco,obs)
)
    bot.send_message(chat_id=update.message.chat_id, text=message)
def almoco_veg_bot(bot,update):
    atualiza_cardapio()
    user_name = update.message.chat.first_name
    message= """
Olá %s,
Almoço Veg de %s (%s) teremos:
-Guarnição: %s

-Prato Principal: %s

-Salada: %s

-Sobremesa: %s

-Suco: %s

Obs: %s"""%(
        user_name,
        "%d/%d/%d"%(dia_cardapio[0],dia_cardapio[1],dia_cardapio[2]),
        dia_semana,
        extrai_refeicao(almoco_veg,guarnicao),
        extrai_refeicao(almoco_veg,prato_principal),
        extrai_refeicao(almoco_veg,salada),
        extrai_refeicao(almoco_veg,sobremesa),
        extrai_refeicao(almoco_veg,suco),
        extrai_refeicao(almoco_veg,obs)
)
    bot.send_message(chat_id=update.message.chat_id, text=message)
def jantar_bot(bot,update):
    atualiza_cardapio()
    user_name = update.message.chat.first_name
    message= """
Olá %s,
Jantar de %s (%s) teremos:
-Guarnição: %s

-Prato Principal: %s

-Salada: %s

-Sobremesa: %s

-Suco: %s

Obs: %s"""%(
        user_name,
        "%d/%d/%d"%(dia_cardapio[0],dia_cardapio[1],dia_cardapio[2]),
        dia_semana,
        extrai_refeicao(jantar,guarnicao),
        extrai_refeicao(jantar,prato_principal),
        extrai_refeicao(jantar,salada),
        extrai_refeicao(jantar,sobremesa),
        extrai_refeicao(jantar,suco),
        extrai_refeicao(jantar,obs)
)
    bot.send_message(chat_id=update.message.chat_id, text=message)
def jantar_veg_bot(bot,update):
    atualiza_cardapio()
    user_name = update.message.chat.first_name
    message= """
Olá %s,
Jantar Veg de %s (%s) teremos:
-Guarnição: %s

-Prato Principal: %s

-Salada: %s

-Sobremesa: %s

-Suco: %s

Obs: %s"""%(
        user_name,
        "%d/%d/%d"%(dia_cardapio[0],dia_cardapio[1],dia_cardapio[2]),
        dia_semana,
        extrai_refeicao(jantar_veg,guarnicao),
        extrai_refeicao(jantar_veg,prato_principal),
        extrai_refeicao(jantar_veg,salada),
        extrai_refeicao(jantar_veg,sobremesa),
        extrai_refeicao(jantar_veg,suco),
        extrai_refeicao(jantar_veg,obs)
)
    bot.send_message(chat_id=update.message.chat_id, text=message)
def cafe_bot(bot,update):
    atualiza_cardapio()
    user_name = update.message.chat.first_name
    message= """
Olá %s,
Café da Manhã de %s (%s) teremos:
%s """%(
        user_name,
        "%d/%d/%d"%(dia_cardapio[0],dia_cardapio[1],dia_cardapio[2]),
        dia_semana,
        cafe
)
    bot.send_message(chat_id=update.message.chat_id, text=message)


def get_temp(bot,update):
    client.connect("mqtt.demo.konkerlabs.net", 1883)
    client.publish(publishAddress,  json.dumps("temp"))

    client_internal = mqtt.Client()
    client_internal.username_pw_set(user, password)
    client_internal.connect("mqtt.demo.konkerlabs.net", 1883)
    client_internal.subscribe(subscribeAddress)
    def on_message(client,data,msg):
        message = msg.payload.decode() + "C"
        bot.send_message(chat_id=update.message.chat_id, text=message)
        client.loop_stop()
    client_internal.on_message = on_message
    client_internal.loop_start()
def get_hum(bot,update):
    client.connect("mqtt.demo.konkerlabs.net", 1883)
    client.publish(publishAddress,  json.dumps("hum"))

    client_internal = mqtt.Client()
    client_internal.username_pw_set(user, password)
    client_internal.connect("mqtt.demo.konkerlabs.net", 1883)
    client_internal.subscribe(subscribeAddress)
    def on_message(client,data,msg):
        message = msg.payload.decode() + "%"
        bot.send_message(chat_id=update.message.chat_id, text=message)
        client.loop_stop()
    client_internal.on_message = on_message
    client_internal.loop_start()

def get_light(bot,update):
    client.connect("mqtt.demo.konkerlabs.net", 1883)
    client.publish(publishAddress,  json.dumps("light"))

    client_internal = mqtt.Client()
    client_internal.username_pw_set(user, password)
    client_internal.connect("mqtt.demo.konkerlabs.net", 1883)
    client_internal.subscribe(subscribeAddress)
    def on_message(client,data,msg):
        message = str(msg.payload)
        bot.send_message(chat_id=update.message.chat_id, text=message)
        client.loop_stop()
    client_internal.on_message = on_message
    client_internal.loop_start()


def publish(bot,update):
    client.connect("mqtt.demo.konkerlabs.net", 1883)
    client.publish(publishAddress,
                 json.dumps({"temperature": "BOOM!", "unit": "celsius"}))
    bot.send_message(chat_id=update.message.chat_id, text="BOOM!")
    print("BOOM!")
def print_display():
    return "PRINT DISP"

updater = Updater(token=token_vina)
dispatcher =  updater.dispatcher
#publish_handler = CommandHandler('publish',publish)
get_temp_handler= CommandHandler('temp',get_temp)
get_hum_handler= CommandHandler('hum',get_hum)
get_light_handler= CommandHandler('light',get_light)
almoco_handler = CommandHandler('almoco',almoco_bot)
almoco_veg_handler = CommandHandler('almoco_veg',almoco_veg_bot)
jantar_handler = CommandHandler('jantar',jantar_bot)
jantar_veg_handler = CommandHandler('jantar_veg',jantar_veg_bot)
cafe_handler = CommandHandler('cafe',cafe_bot)
start_handler = CommandHandler('start',start)
dispatcher.add_handler(almoco_handler)
dispatcher.add_handler(almoco_veg_handler)
dispatcher.add_handler(jantar_handler)
dispatcher.add_handler(jantar_veg_handler)
dispatcher.add_handler(cafe_handler)
dispatcher.add_handler(start_handler)
dispatcher.add_handler(get_temp_handler)
dispatcher.add_handler(get_hum_handler)
dispatcher.add_handler(get_light_handler)
#dispatcher.add_handler(publish_handler)
updater.start_polling()

#updater.stop()
#updater.idle()


