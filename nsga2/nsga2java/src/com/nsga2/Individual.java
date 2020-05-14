package src.com.nsga2;

import java.util.Set;

class Individual {
    int rank;
    double crowding_distance;
    Set<Individual> dominates;
    double[] objectives;
    int ndominated;

    public void set_objectives(double[] objectives) {
        this.objectives = objectives;
    }
}
