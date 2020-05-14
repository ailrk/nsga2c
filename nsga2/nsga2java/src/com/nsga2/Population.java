package src.com.nsga2;

import java.util.ArrayList;
import java.util.Collection;

class Population {
    ArrayList<Individual> population;
    ArrayList<Individual> fronts;

    Population() {
       population = new ArrayList<>();
       fronts = new ArrayList<>();
    }

    public int size() {
        return population.size();
    }

    public void extend(Collection<Individual> new_individuals) {
        population.addAll(new_individuals);
    }
}
