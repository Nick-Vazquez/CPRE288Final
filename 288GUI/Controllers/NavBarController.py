import logging
import queue

from Components.NavBar import NavBar
from Models.CyBotMessage import OpModeMessage
from Models.NavBarCallbacks import *
from Services.CyBotMessageService import Subscriber
from Services.OpModeService import OpModeService

logger = logging.getLogger(__name__)


class NavBarController(Subscriber):

    def __init__(self, parent, opmode_srv: OpModeService):
        super(NavBarController, self).__init__()
        self.parent = parent
        self.accepts = OpModeMessage
        self._opmode_srv = opmode_srv
        self.view = NavBar(parent, self.setup_callbacks())

    @property
    def opmode_srv(self):
        return self.opmode_srv

    @opmode_srv.setter
    def opmode_srv(self, value):
        self._opmode_srv = value


    def setup_callbacks(self):
        nav_callbacks = NavSectionCallbacks(lambda x=0: print('Home'),
                                            lambda x=0: print('Console'),
                                            lambda x=0: print('About'))
        opmode_callbacks = OpModeCallbacks(self._opmode_srv.teleop,
                                           self._opmode_srv.auto)
        control_callbacks = ControlSectionCallbacks(
            lambda x=0: print('Start'),
            lambda x=0: print('Stop'))

        return NavBarCallbacks(nav_callbacks, opmode_callbacks,
                               control_callbacks)

    def poll_input_queue(self):
        try:
            msg = self.input_queue.get_nowait()
            msg: OpModeMessage
            logger.info(f'OpMode: {msg.op_mode}')
        except queue.Empty:
            pass
