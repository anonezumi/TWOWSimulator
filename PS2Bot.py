import discord
import asyncio
import random

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
        responses.append(f.readline())
    f.close()

@client.event
async def on_message(message):
    global rnum
    global responses
    if (message.content == "!response") & message.channel.is_private:
        num = random.randint(0, rnum - 1)
        await client.send_message(message.channel, responses[num])
    elif (message.content == "!die") & (message.author.id == "193874867324583936"):
        exit(0)
		
client.run("lolyourenotgettingmytokenbitch")
