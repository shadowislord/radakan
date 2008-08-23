package com.radakan.res;

import java.io.IOException;
import java.lang.ref.PhantomReference;
import java.lang.ref.Reference;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadFactory;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Changes recently made by Tomygun:
 *	addCache no longer requieres the firt argument since it wasn't neccesary
 *  loadResources added with var args... i dunno might be useful, might be removed...
 * @author Momoko_Fan
 */
public class ResourceManager {

    protected static volatile Set<String> loadingResourcesSet = new HashSet<String>();
	protected static volatile Map<String, Reference<?>> cacheMap = new HashMap<String, Reference<?>>();
	protected static final Map<Class<?>, IResourceLoader> loaderMap = new HashMap<Class<?>, IResourceLoader>();
	protected static final ExecutorService executor = Executors.newSingleThreadExecutor(new LoadingThreadFactory());
	protected static IFileSystem fileSystem;
	

	protected static class LoadingThreadFactory implements ThreadFactory {

		protected int id = 0;

		public Thread newThread(Runnable r) {
			Thread t = new Thread(r, "pool" + (id++));
			t.setDaemon(true);
			t.setPriority(Thread.MIN_PRIORITY);
			return t;
		}
	}

	protected static <T> void addToCache(String resourceName, T value) {
		if (cacheMap.containsKey(resourceName)) {
			return;
		}
		loadingResourcesSet.remove(resourceName);
		Reference<T> ref = new WeakReference<T>(value);
		cacheMap.put(resourceName, ref);
	}

	protected static <T> void removeFromCache(String resourceName) {
		if (!cacheMap.containsKey(resourceName)) {
			return;
		}
		cacheMap.remove(resourceName);
	}

	protected static <T> Object getFromCache(Class<?> resourceType, String resourceName) {
		@SuppressWarnings("unchecked")
		Reference<T> ref = (Reference<T>) cacheMap.get(resourceName);
		if (ref == null) {
			return null;
		}
		return ref.get();
	}

	protected static void removeExpiredFromCache() {
		for (Map.Entry<String, Reference<?>> entry : cacheMap.entrySet()) {
			if (entry.getValue().get() == null) {
				cacheMap.remove(entry.getKey());
			}
		}
	}

	protected static void onException(Exception ex) {
		ex.printStackTrace();
	}

	public static void setFileSystem(IFileSystem fileSystem) {
		ResourceManager.fileSystem = fileSystem;
	}

	public static <T> void registerLoader(Class<T> type, IResourceLoader<T> loader) {
		loaderMap.put(type, loader);
	}

	public static IFileSystem getFileSystem() {
		return fileSystem;
	}

	public static boolean isLoading(String resourceName) {
		return loadingResourcesSet.contains(resourceName);
	}

        public static boolean isIdle(){
            return loadingResourcesSet.size() == 0;
        }
        
	public static boolean isLoadingThread() {
		return Thread.currentThread().getName().startsWith("pool");
	}

	public static <T> T loadResource(Class<T> resourceType, String resourceName) {
		try {
			@SuppressWarnings("unchecked")
			WeakReference<T> ref = (WeakReference<T>) cacheMap.get(resourceName);

			if (ref != null) {
				if (ref.get() != null) {
					return ref.get();
				} else {
					cacheMap.remove(ref);
				}
			}
			@SuppressWarnings("unchecked")
			IResourceLoader<T> loader = loaderMap.get(resourceType);

			loadingResourcesSet.add(resourceName);
			T val = loader.load(resourceName);
			loadingResourcesSet.remove(resourceName);
                        if (loader.useCache()) addToCache(resourceName, val);

			return val;
		} catch (IOException ex) {
			onException(ex);
		}

		return null;
	}
	
	public static <T> List<T> loadResources(Class<T> resourceType, String ... resourceNames) {
		List<T> loadedResources = new ArrayList<T>();
		for (int i = 0; i < resourceNames.length; i++) {
			loadedResources.add(loadResource(resourceType, resourceNames[i]));
		}
		return loadedResources;
	}

	public static <T> void loadResourceLater(final Class<T> resourceType, final String resourceName) {
		@SuppressWarnings("unchecked")
		WeakReference<T> ref = (WeakReference<T>) cacheMap.get(resourceName);

		if (ref != null) {
			if (ref.get() != null) {
				return;
			} else {
				cacheMap.remove(ref);
			}
		}
                
                // prepeare to load resource in loading thread
                loadingResourcesSet.add(resourceName);
                
		@SuppressWarnings("unchecked")
		final IResourceLoader<T> loader = loaderMap.get(resourceType);
                
		executor.submit(new Callable<Object>() {
			public Object call() throws ExecutionException {
				try {
                                    T val = loader.load(resourceName);
                                    if (loader.useCache()){
					addToCache(resourceName, val);
                                    }
                                    loadingResourcesSet.remove(resourceName);
				} catch (IOException ex) {
					onException(ex);
				}

				return null;
			}
		});
	}
        
        public static <T> Future<T> runTaskLater(final String taskName, final Callable<T> callable){
            loadingResourcesSet.add(taskName);
            return executor.submit(new Callable<T>(){
                public T call() throws Exception{
                    T obj = callable.call();
                    loadingResourcesSet.remove(taskName);
                    return obj;
                }
            });
        }

	@SuppressWarnings("unchecked")
   public static <T> T aquireResource(Class<T> resourceType, String resourceName) {
        try {
            IResourceLoader<T> loader = loaderMap.get(resourceType);
            if (loader.useCache()) {
                return (T) getFromCache(resourceType, resourceName);
            } else {
                return loader.load(resourceName);
            }
        } catch (IOException ex) {
            Logger.getLogger(ResourceManager.class.getName()).log(Level.SEVERE, null, ex);
        }
        return null;
    }
}
