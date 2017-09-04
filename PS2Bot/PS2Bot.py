import discord
import asyncio
import random
import time

client = discord.Client()
responses = []
f = open("responses.txt", "r")
rnum = 0

@client.event
async def on_ready():
    global rnum
    global responses
    for line in f:
        rnum = rnum + 1
        responses.append(line)
    f.close()
    print("processed responses.")

@client.event
async def on_message(message):
    global rnum
    global responses
    if (message.content == "!response") & message.channel.is_private:
        num = random.randint(0, rnum - 1)
        await client.send_message(message.channel, responses[num])
    elif (message.content == "!die") & (message.author.id == "193874867324583936"):
        exit(0)
    elif (message.content == "!read") & (message.author.id == "193874867324583936"):
        f1 = open("script.txt", "r")
        script = []
        timing = []
        length = 0
        for line in f1:
            length = length + 1
            timing.append(float(line[:4]))
            script.append(line[5:])
        print("processed script.")
        i = 0
        while i < length:
            time.sleep(timing[i]/1000.0)
            await client.send_message(message.channel, script[i])
            i = i + 1
		
client.run("MzUyNTM2NTIyMjExMTk2OTI4.DIil8g.o3tU6F_3bEKqy-bOIFImoeriONc")
