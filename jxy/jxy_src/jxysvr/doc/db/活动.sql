

ALTER TABLE `activityencouragepro`
ADD COLUMN `JsonValues`  varchar(1024) NOT NULL DEFAULT '' AFTER `ActivityDesc`;


UPDATE activityencouragepro set JsonValues = '
{
    "item": [
        {
            "value": 100,
            "num": 100
        }
    ],
    "hero": 1,
    "coin": 100,
    "gold": 100,
    "phystrength": 100,
    "science": 100,
    "story": 100,
    "bluegas": 100,
    "pruplegas": 100,
    "jingjie": 100,
    "doorstribute": 0,
    "factionfunds": 0,
    "factiontaskintegral": 0
}'