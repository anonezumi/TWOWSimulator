import discord
import asyncio
import random
import time
from PIL import ImageFont, ImageDraw, Image

client = discord.Client()
responses = []
f = open("responses.txt", "r")
rnum = 0
gp = ImageFont.truetype("Graphpaper-Regular.ttf", 20)


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
        processlb = False
        lb = []
        while i < length:
            if processlb:
                lb.append(script[i])
                if script[i] == "[ENDLB]\n":
                    processlb = False
                    await create_lb(message.channel, lb)
            else:
                if script[i] == "[STARTLB]\n":
                    processlb = True
                else:
                    await asyncio.sleep(timing[i] / 1000.0)
                    await client.send_message(message.channel, script[i])
            i = i + 1
    elif (message.content == "!lb") & (message.author.id == "193874867324583936"):
        await create_lb(message.channel)


async def create_lb(channel, lb):
    global rnum
    global responses
    lines = []
    currentline = []
    length = 0
    mode = 0
    done = False
    while done == False:
        line = lb[length * 3 + mode]
        if mode == 2:
            lines.append(currentline)
            currentline = []
            mode = 0
            length = length + 1
            if line == "[ENDLB]\n":
                done = True
        else:
            currentline.append(line)
            mode = mode + 1
    lb = Image.new("RGB", (800, 20 * length), (192, 192, 192))
    d = ImageDraw.Draw(lb)
    i = 0
    while i < length:
        if i / length <= 0.1:
            d.rectangle([(0, i * 20), (750, (i + 1) * 20)], fill=(255, 255, 128))
        elif (i / length < 0.8) & (i != length - 1):
            d.rectangle([(0, i * 20), (750, (i + 1) * 20)], fill=(128, 255, 128))
        else:
            d.rectangle([(0, i * 20), (750, (i + 1) * 20)], fill=(255, 128, 128))
        score = float(lines[i][1])
        r1 = 64 + int(abs(score - 50) / 50) * 192
        g1 = 255
        if score < 50: g1 = 128 + int((score / 50) * 192)
        d.rectangle([(750, i * 20), (800, (i + 1) * 20)], fill=(r1, g1, 128))
        d.text((0, i * 20), str(i + 1), fill=(0, 0, 0), font=gp)
        d.text((50, i * 20), lines[i][0], fill=(0, 0, 0), font=gp)
        d.text((150, i * 20), responses[random.randint(0, rnum - 1)], fill=(0, 0, 0), font=gp)
        d.text((750, i * 20), lines[i][1], fill=(0, 0, 0), font=gp)
        d.line([(0, i * 20), (800, i * 20)], fill=(128, 128, 255))
        i = i + 1
    d.line([(100, 0), (100, 20 * length)], fill=(128, 128, 255))
    d.line([(700, 0), (700, 20 * length)], fill=(128, 128, 255))
    d.line([(750, 0), (750, 20 * length)], fill=(128, 128, 255))
    print("leaderboard complete.")
    lb.save("leaderboard.png")
    await client.send_file(channel, "leaderboard.png")


client.run("MzUyNTM2NTIyMjExMTk2OTI4.DIil8g.o3tU6F_3bEKqy-bOIFImoeriONc")
