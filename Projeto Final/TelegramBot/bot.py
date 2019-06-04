import logging
import telegram
from telegram.ext import CommandHandler
from telegram.ext import Updater
import paho.mqtt.client as mqtt
from bandeco import *

# Funções de Handler do BOT
def start(bot,update):
    user_name = update.message.chat.first_name
    message= "Hello %s\n"%(user_name)
    bot.send_message(chat_id=update.message.chat_id, text=message)
    print(update)
def almoco_bot(bot,update):
    global bandeco
    bandeco.atualiza_cardapio()
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
        "%d/%d/%d"%(bandeco.dia_cardapio[0],bandeco.dia_cardapio[1],bandeco.dia_cardapio[2]),
        bandeco.dia_semana,
        bandeco.extrai_refeicao(bandeco.almoco,bandeco.guarnicao),
        bandeco.extrai_refeicao(bandeco.almoco,bandeco.prato_principal),
        bandeco.extrai_refeicao(bandeco.almoco,bandeco.salada),
        bandeco.extrai_refeicao(bandeco.almoco,bandeco.sobremesa),
        bandeco.extrai_refeicao(bandeco.almoco,bandeco.suco),
        bandeco.extrai_refeicao(bandeco.almoco,bandeco.obs)
)
    bot.send_message(chat_id=update.message.chat_id, text=message)
def almoco_veg_bot(bot,update):
    bandeco.atualiza_cardapio()
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
        "%d/%d/%d"%(bandeco.dia_cardapio[0],bandeco.dia_cardapio[1],bandeco.dia_cardapio[2]),
        bandeco.dia_semana,
        bandeco.extrai_refeicao(bandeco.almoco_veg,bandeco.guarnicao),
        bandeco.extrai_refeicao(bandeco.almoco_veg,bandeco.prato_principal),
        bandeco.extrai_refeicao(bandeco.almoco_veg,bandeco.salada),
        bandeco.extrai_refeicao(bandeco.almoco_veg,bandeco.sobremesa),
        bandeco.extrai_refeicao(bandeco.almoco_veg,bandeco.suco),
        bandeco.extrai_refeicao(bandeco.almoco_veg,bandeco.obs)
)
    bot.send_message(chat_id=update.message.chat_id, text=message)
def jantar_bot(bot,update):
    global bandeco
    bandeco.atualiza_cardapio()
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
        "%d/%d/%d"%(bandeco.dia_cardapio[0],bandeco.dia_cardapio[1],bandeco.dia_cardapio[2]),
        bandeco.dia_semana,
        bandeco.extrai_refeicao(bandeco.jantar,bandeco.guarnicao),
        bandeco.extrai_refeicao(bandeco.jantar,bandeco.prato_principal),
        bandeco.extrai_refeicao(bandeco.jantar,bandeco.salada),
        bandeco.extrai_refeicao(bandeco.jantar,bandeco.sobremesa),
        bandeco.extrai_refeicao(bandeco.jantar,bandeco.suco),
        bandeco.extrai_refeicao(bandeco.jantar,bandeco.obs)
)
    bot.send_message(chat_id=update.message.chat_id, text=message)
def jantar_veg_bot(bot,update):
    global bandeco
    bandeco.atualiza_cardapio()
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
        "%d/%d/%d"%(bandeco.dia_cardapio[0],bandeco.dia_cardapio[1],bandeco.dia_cardapio[2]),
        bandeco.dia_semana,
        bandeco.extrai_refeicao(bandeco.jantar_veg,bandeco.guarnicao),
        bandeco.extrai_refeicao(bandeco.jantar_veg,bandeco.prato_principal),
        bandeco.extrai_refeicao(bandeco.jantar_veg,bandeco.salada),
        bandeco.extrai_refeicao(bandeco.jantar_veg,bandeco.sobremesa),
        bandeco.extrai_refeicao(bandeco.jantar_veg,bandeco.suco),
        bandeco.extrai_refeicao(bandeco.jantar_veg,bandeco.obs)
)
    bot.send_message(chat_id=update.message.chat_id, text=message)
def cafe_bot(bot,update):
    global bandeco
    bandeco.atualiza_cardapio()
    user_name = update.message.chat.first_name
    message= """
Olá %s,
Café da Manhã de %s (%s) teremos:
%s """%(
        user_name,
        "%d/%d/%d"%(bandeco.dia_cardapio[0],bandeco.dia_cardapio[1],bandeco.dia_cardapio[2]),
        bandeco.dia_semana,
        bandeco.cafe
)
    bot.send_message(chat_id=update.message.chat_id, text=message)

def publish(bot,update):
    client.connect("mqtt.demo.konkerlabs.net", 1883)
    client.publish(publishAddress,
                 json.dumps({"temperature": "BOOM!", "unit": "celsius"}))
    bot.send_message(chat_id=update.message.chat_id, text="BOOM!")
    print("BOOM!")
def get_temp(bot,update):
    bot.send_message(chat_id=update.message.chat_id, text="BOOM!")
    print("TEMP = 20")
def get_hum():
    return "HUM = 20"
def print_display():
    return "PRINT DISP"





# Setting up logger
logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger()
logger.setLevel(logging.INFO)
#token_telegram = '666164429:AAFPkMnbqRKXqCPvv-OtycJ5K5y4RQ3k_U4'
token_vina = '850097657:AAHEHdl6HrpDYKb2_9nKPFDx1eQkBMyW7xg'
bot = telegram.Bot(token=token_vina)

# CLIENT MQTT
user = "ou83rio3h43j"
password = "anioaDro9R87"
publishAddress = "data/ou83rio3h43j/pub/out"
client = mqtt.Client()
client.username_pw_set(user, password)

global bandeco 
bandeco = Bandeco()

updater = Updater(token=token_vina)
dispatcher = updater.dispatcher
#publish_handler = CommandHandler('publish',publish)
almoco_handler = CommandHandler('almoco', almoco_bot)
almoco_veg_handler = CommandHandler('almoco_veg', almoco_veg_bot)
jantar_handler = CommandHandler('jantar', jantar_bot)
jantar_veg_handler = CommandHandler('jantar_veg', jantar_veg_bot)
cafe_handler = CommandHandler('cafe', cafe_bot)
start_handler = CommandHandler('start', start)
dispatcher.add_handler(almoco_handler)
dispatcher.add_handler(almoco_veg_handler)
dispatcher.add_handler(jantar_handler)
dispatcher.add_handler(jantar_veg_handler)
dispatcher.add_handler(cafe_handler)
dispatcher.add_handler(start_handler)
# dispatcher.add_handler(publish_handler)
updater.start_polling()

#updater.idle()
#updater.stop()
#
