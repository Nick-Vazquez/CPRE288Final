import enum
import typing
import pydantic
from pydantic import Field


class ScanUpdateType(str, enum.Enum):
    """Differentiates between update types from distance sensors."""
    IR = 'IR'
    PING = 'PING'


class MovementDirection(str, enum.Enum):
    """Define what direction to move the bot.

    FORWARD/REVERSE - CM
    CLOCKWISE/C_CLOCKWISE - DEG
    """
    FORWARD = 'FORWARD'
    REVERSE = 'REVERSE'
    CLOCKWISE = 'CLOCKWISE'
    C_CLOCKWISE = 'C_CLOCKWISE'


class OpModes(str, enum.Enum):
    TELEOP = "TELEOP"
    AUTONOMOUS = "AUTONOMOUS"


class CyBotMessage(pydantic.BaseModel):
    mes_type: int = Field(0, const=True)


class OpModeMessage(CyBotMessage):
    mes_type: int = Field(1, const=True)


class MoveMessage(CyBotMessage):
    """Message that holds movement information to/from CYBOT."""
    mes_type: int = Field(2, const=True)
    direction: MovementDirection
    magnitude: int


class RequestScanMessage(CyBotMessage):
    """GUI -> Bot"""
    mes_type: int = Field(3, const=True)
    full: bool
    angles: typing.List[int]


class ScanResultsMessage(CyBotMessage):
    """Bot -> GUI"""
    mes_type: int = Field(4, const=True)
    update_type: ScanUpdateType
    angles: typing.List[int] = []
    distances: typing.List[float] = []


class BayResultsMessage(CyBotMessage):
    mes_type: int = Field(5, const=True)
    bay_num: int
    closed: bool
    occupancy: int


class SoundMessage(CyBotMessage):
    mes_type: int = Field(6, const=True)
    music_number: int


# I hate myself for this
message_mapping = [CyBotMessage, OpModeMessage, MoveMessage, RequestScanMessage,
                   ScanResultsMessage, BayResultsMessage, SoundMessage]




if __name__ == '__main__':
    message = ScanResultsMessage(
        update_type=ScanUpdateType.IR, angles=[5, 10]).json()
    print(message)
