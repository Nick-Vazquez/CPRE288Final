import logging
import queue

from Services.CyBotMessageService import Subscriber
from Components.ScanPlotterView import PlotterView
from Models.ScanResults import ScanResult
from Models.CyBotMessage import ScanResultsMessage, ScanUpdateType

logger = logging.getLogger(__name__)


class PlotterController(Subscriber):
    def __init__(self, parent):
        super(PlotterController, self).__init__()
        self.parent = parent
        self.accepts = ScanResultsMessage
        self.ir_results = ScanResult()
        self.ir_results.result = [0 for _ in range(90)]
        self.view = PlotterView(parent, self.ir_results)
        self.poll_input_queue()

    def poll_input_queue(self):
        try:
            msg = self.input_queue.get_nowait()
            msg: ScanResultsMessage
            logger.debug(f"Updating distances: {msg.distances}")
            for index, angle in enumerate(msg.angles):
                distance = msg.distances[index]
                self.ir_results.set_result_at_angle(angle, distance)
        except queue.Empty:
            pass
        self.parent.after(200, self.poll_input_queue)
