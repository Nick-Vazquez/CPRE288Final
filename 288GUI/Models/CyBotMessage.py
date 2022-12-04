"""
Defines messages that are sent between the CyBOT and GUI.

__author__ : Nick Vazquez
__created__: 22/11/12
"""
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
    """Operational modes the bot can be in. Operator controlled or
    Autonomous."""
    TELEOP = "TELEOP"
    AUTONOMOUS = "AUTONOMOUS"


class CyBotMessage(pydantic.BaseModel):
    """Base message that is inherited for all functional messages."""
    mes_type: int = Field(0, const=True)


class OpModeMessage(CyBotMessage):
    """ GUI -> CyBot
    Commands the CyBot to switch into operator or autonomous mode.
    """
    mes_type: int = Field(1, const=True)
    op_mode: OpModes


class MoveMessage(CyBotMessage):
    """ GUI -> CyBot
    Message that holds movement information to/from CYBOT.
    Magnitude is dependent on direction selected. Forward/Backward is in/cm,
    while Clockwise/C_Clockwise is degrees."""
    mes_type: int = Field(2, const=True)
    direction: MovementDirection
    magnitude: int


class RequestScanMessage(CyBotMessage):
    """GUI -> CyBot
    Commands the CyBot to perform a scan. This should only be available in
    TeleOp mode.
    """
    mes_type: int = Field(3, const=True)
    full: bool
    angles: typing.List[int] = []


class ScanResultsMessage(CyBotMessage):
    """CyBot -> GUI
    Alerts the GUI of updated results of a scan, and the sensor used to scan.
    """
    mes_type: int = Field(4, const=True)
    update_type: ScanUpdateType
    angles: typing.List[int] = []
    distances: typing.List[float] = []


class BayResultsMessage(CyBotMessage):
    """CyBot -> GUI
    Alerts the GUI of a completed room scan and the status of the bay
    scanned."""
    mes_type: int = Field(5, const=True)
    bay_num: int
    closed: bool
    occupancy: int


class SoundMessage(CyBotMessage):
    """GUI -> CyBot
    Commands the CyBot to play a sound based on the music number provided."""
    mes_type: int = Field(6, const=True)
    music_number: int


class CliffSensorMessage(CyBotMessage):
    """CyBot -> GUI
    Alerts the GUI of a cliff that was just found, and the sensors that
    detected the cliff."""
    mes_type: int = Field(7, const=True)
    sensor_num: typing.List[int] = []


# I hate myself for this
message_mapping = [CyBotMessage, OpModeMessage, MoveMessage, RequestScanMessage,
                   ScanResultsMessage, BayResultsMessage, SoundMessage,
                   CliffSensorMessage]

if __name__ == '__main__':
    op_mode = OpModeMessage(op_mode=OpModes.TELEOP)
    move = MoveMessage(direction=MovementDirection.CLOCKWISE,
                       magnitude=20)
    request_scan = RequestScanMessage(full=True)
    scan_results = ScanResultsMessage(update_type=ScanUpdateType.IR,
                                      angles=[5, 10])
    bay_results = BayResultsMessage(bay_num=3, closed=False,
                                    occupancy=12)
    sound = SoundMessage(music_number=3)
    cliff = CliffSensorMessage(sensor_num=[1, 2])

    print(op_mode.json())
    print(move.json())
    print(request_scan.json())
    print(scan_results.json())
    print(bay_results.json())
    print(sound.json())
    print(cliff.json())
