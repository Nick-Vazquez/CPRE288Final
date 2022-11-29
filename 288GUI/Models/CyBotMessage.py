import enum
import typing
import pydantic
from pydantic import Field


class ScanUpdateType(enum.Enum):
    """Differentiates between update types from distance sensors."""
    IR = enum.auto()
    PING = enum.auto()


class MovementDirection(enum.Enum):
    """Define what direction to move the bot.

    FORWARD/REVERSE - CM
    CLOCKWISE/C_CLOCKWISE - DEG
    """
    FORWARD = enum.auto()
    REVERSE = enum.auto()
    CLOCKWISE = enum.auto()
    C_CLOCKWISE = enum.auto()


class CyBotMessage(pydantic.BaseModel):
    mes_type: int = Field(0, const=True)


class ScanMessage(CyBotMessage):
    mes_type: int = Field(1, const=True)
    update_type: ScanUpdateType
    angles: typing.List[int] = []
    distances: typing.List[float] = []


class MoveMessage(CyBotMessage):
    """Message that holds movement information to/from CYBOT."""
    mes_type: int = Field(2, const=True)
    direction: MovementDirection
    magnitude: int


# I hate myself for this
message_mapping = [CyBotMessage, ScanMessage, MoveMessage]


if __name__ == '__main__':
    message = ScanMessage(update_type=ScanUpdateType.IR, angles=[5, 10])
    print(message)
