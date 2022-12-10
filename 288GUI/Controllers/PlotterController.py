"""
Acts as interface between plotter view and the external GUI application.

__created__ = 2022/12/05
__author__ = Nick Vazquez (nmv)
"""
import logging
import queue

from Components.ScanPlotterView import PlotterView
from Models.CyBotMessage import ScanResultsMessage
from Models.ScanResults import ScanResult
from Services.CyBotMessageService import Subscriber

logger = logging.getLogger(__name__)


class PlotterController(Subscriber):
    """Holds and controls the model and view for plotting scan results."""
    def __init__(self, parent):
        super(PlotterController, self).__init__()
        self.parent = parent
        self.accepts = ScanResultsMessage
        self.ir_results = ScanResult()
        self.ir_results.result = [0 for _ in range(90)]
        self.view = PlotterView(parent, self.ir_results)
        self.poll_input_queue()

    def poll_input_queue(self):
        """Checks the input queue for any updated ScanResultsMessages."""
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
